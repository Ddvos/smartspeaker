#include "audio_codecs.h"
#include "display_driver.h"
#include "driver/i2c_master.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_codec_dev.h"
#include "esp_codec_dev_defaults.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "audio_codecs";

// GPIO assignments for Waveshare ESP32-P4-WIFI6-Touch-LCD
#define I2S_MCK_IO      GPIO_NUM_13
#define I2S_BCK_IO      GPIO_NUM_12
#define I2S_WS_IO       GPIO_NUM_10
#define I2S_DO_IO       GPIO_NUM_9
#define I2S_DI_IO       GPIO_NUM_11
#define I2S_NUM         0

#define GPIO_PA_EN      GPIO_NUM_53

// Direct I2S handles
static i2s_chan_handle_t s_tx_handle = NULL;
static i2s_chan_handle_t s_rx_handle = NULL;

// I2C device handle for ES8311 direct register access
static i2c_master_dev_handle_t s_es8311_dev = NULL;

// ES7210 mic via esp_codec_dev (proper driver, doesn't corrupt I2S TX)
static esp_codec_dev_handle_t s_mic_dev = NULL;

// ── I2C register helpers ──────────────────────────────────────

static esp_err_t es8311_write_reg(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = { reg, val };
    return i2c_master_transmit(s_es8311_dev, buf, 2, 100);
}

static esp_err_t es8311_read_reg(uint8_t reg, uint8_t *val)
{
    return i2c_master_transmit_receive(s_es8311_dev, &reg, 1, val, 1, 100);
}


// ── Initialization matching Waveshare example EXACTLY ─────────

static esp_err_t codec_init(void)
{
    i2c_master_bus_handle_t i2c_bus = display_driver_get_i2c_bus();
    ESP_RETURN_ON_FALSE(i2c_bus, ESP_ERR_INVALID_STATE, TAG, "I2C bus not initialized");

    // Step 1: PA GPIO - configure and enable FIRST (matching Waveshare example)
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GPIO_PA_EN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    gpio_set_level(GPIO_PA_EN, 1);
    ESP_LOGI(TAG, "PA amplifier enabled (GPIO%d)", GPIO_PA_EN);

    // Step 2: I2S init (matching Waveshare example exactly)
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM, I2S_ROLE_MASTER);
    chan_cfg.auto_clear = true;
    ESP_RETURN_ON_ERROR(i2s_new_channel(&chan_cfg, &s_tx_handle, &s_rx_handle),
                        TAG, "I2S channel create failed");

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(AUDIO_SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_MCK_IO,
            .bclk = I2S_BCK_IO,
            .ws = I2S_WS_IO,
            .dout = I2S_DO_IO,
            .din = I2S_DI_IO,
            .invert_flags = { .mclk_inv = false, .bclk_inv = false, .ws_inv = false },
        },
    };
    std_cfg.clk_cfg.mclk_multiple = AUDIO_MCLK_MULTIPLE;

    ESP_RETURN_ON_ERROR(i2s_channel_init_std_mode(s_tx_handle, &std_cfg), TAG, "I2S TX init failed");
    ESP_RETURN_ON_ERROR(i2s_channel_init_std_mode(s_rx_handle, &std_cfg), TAG, "I2S RX init failed");
    ESP_RETURN_ON_ERROR(i2s_channel_enable(s_tx_handle), TAG, "I2S TX enable failed");
    ESP_RETURN_ON_ERROR(i2s_channel_enable(s_rx_handle), TAG, "I2S RX enable failed");
    ESP_LOGI(TAG, "I2S initialized: %dHz, 16-bit, stereo, MCLK=%dx", AUDIO_SAMPLE_RATE, AUDIO_MCLK_MULTIPLE);

    // Step 3: ES8311 codec init via direct I2C (matching Waveshare es8311 library)
    i2c_device_config_t es8311_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = ES8311_I2C_ADDR,
        .scl_speed_hz = 100000,
    };
    ESP_RETURN_ON_ERROR(i2c_master_bus_add_device(i2c_bus, &es8311_cfg, &s_es8311_dev),
                        TAG, "ES8311 I2C add failed");
    ESP_RETURN_ON_ERROR(i2c_master_probe(i2c_bus, ES8311_I2C_ADDR, 100),
                        TAG, "ES8311 not found");
    ESP_LOGI(TAG, "ES8311 detected at 0x%02X", ES8311_I2C_ADDR);

    // ES8311 init sequence from the espressif/es8311 component source
    // Reset
    es8311_write_reg(0x00, 0x1F);
    vTaskDelay(pdMS_TO_TICKS(20));
    es8311_write_reg(0x00, 0x80);

    // Clock config for MCLK from pin, slave mode
    es8311_write_reg(0x01, 0x3F);  // MCLK on, from MCLK pin

    // From es8311 component coeff_div table for 6144000Hz MCLK / 16000Hz sample rate:
    // pre_div=3, pre_multi=1, adc_div=1, dac_div=1, fs_mode=0, lrck_h=0x00, lrck_l=0xff, bclk_div=4, adc_osr=0x10, dac_osr=0x20
    uint8_t regv;
    es8311_read_reg(0x02, &regv);
    regv &= 0x07;
    regv |= (3 - 1) << 5;  // pre_div = 3
    es8311_write_reg(0x02, regv);

    // Pre-multiplier
    es8311_read_reg(0x02, &regv);
    regv &= 0xE0;
    // pre_multi=1 → datmp=0
    es8311_write_reg(0x02, regv);

    es8311_write_reg(0x03, 0x10);  // adc_div=1, fs_mode=0
    es8311_write_reg(0x16, 0x24);  // ADC volume
    es8311_write_reg(0x04, 0x10);  // adc_osr = 0x10
    es8311_write_reg(0x05, 0x00);  // dac_osr = 0x20 (will be set below)
    es8311_write_reg(0x0B, 0x00);  // System
    es8311_write_reg(0x0C, 0x00);  // System
    es8311_write_reg(0x10, 0x1F);  // Power: VMID + ref
    es8311_write_reg(0x11, 0x7F);  // Power up all
    es8311_write_reg(0x00, 0x80);  // CSM on

    // Slave mode
    es8311_read_reg(0x00, &regv);
    regv &= 0xBF;  // Clear master mode bit
    es8311_write_reg(0x00, regv);

    // MCLK source from pin, not inverted
    es8311_write_reg(0x01, 0x3F);

    // SCLK not inverted
    es8311_read_reg(0x06, &regv);
    regv &= ~0x20;
    es8311_write_reg(0x06, regv);

    es8311_write_reg(0x13, 0x10);  // System
    es8311_write_reg(0x1B, 0x0A);  // ADC
    es8311_write_reg(0x1C, 0x6A);  // ADC
    es8311_write_reg(0x44, 0x08);  // GPIO/pad

    // I2S format: 16-bit, I2S Philips
    // SDP In (to DAC)
    es8311_read_reg(0x09, &regv);
    regv &= 0xC0;
    regv |= 0x0C;  // 16-bit width
    es8311_write_reg(0x09, regv);

    // SDP Out (from ADC)
    es8311_read_reg(0x0A, &regv);
    regv &= 0xC0;
    regv |= 0x0C;  // 16-bit width
    es8311_write_reg(0x0A, regv);

    // Sample rate coefficients for 16kHz with 384x MCLK
    // LRCK divider
    es8311_write_reg(0x07, 0x00);  // lrck_h
    es8311_write_reg(0x08, 0xFF);  // lrck_l

    // BCLK divider
    es8311_read_reg(0x06, &regv);
    regv &= 0xE0;
    regv |= 0x04;  // bclk_div = 4
    es8311_write_reg(0x06, regv);

    // OSR
    es8311_write_reg(0x04, 0x10);  // adc_osr
    es8311_write_reg(0x05, 0x20);  // dac_osr

    // Start DAC
    es8311_write_reg(0x0E, 0x02);  // Enable analog ref
    es8311_write_reg(0x0F, 0x44);  // Power on DAC + ADC
    es8311_write_reg(0x12, 0x00);  // DAC config

    // Set volume to ~90% (register 0x32: 0x00=mute, 0xFF=+32dB)
    // 0xBF ≈ 75% of register range ≈ loud
    es8311_write_reg(0x32, 0xCC);

    // Microphone config (for ES8311's built-in mic - not used but needed for init)
    es8311_write_reg(0x14, 0x1A);  // Power on analog blocks for ADC

    ESP_LOGI(TAG, "ES8311 codec initialized (direct register writes)");

    // Step 4: ES7210 microphone via esp_codec_dev (proven driver)
    // Note: esp_codec_dev for INPUT only doesn't corrupt I2S TX channel
    audio_codec_i2s_cfg_t i2s_cfg = {
        .rx_handle = s_rx_handle,
        .tx_handle = s_tx_handle,
    };
    const audio_codec_data_if_t *data_if = audio_codec_new_i2s_data(&i2s_cfg);
    ESP_RETURN_ON_FALSE(data_if, ESP_FAIL, TAG, "I2S data interface failed");

    audio_codec_i2c_cfg_t es7210_i2c = {
        .addr = ES7210_I2C_ADDR << 1,  // 8-bit address
        .bus_handle = i2c_bus,
    };
    const audio_codec_ctrl_if_t *es7210_ctrl = audio_codec_new_i2c_ctrl(&es7210_i2c);
    ESP_RETURN_ON_FALSE(es7210_ctrl, ESP_FAIL, TAG, "ES7210 I2C ctrl failed");

    es7210_codec_cfg_t es7210_codec_cfg = {
        .ctrl_if = es7210_ctrl,
    };
    const audio_codec_if_t *es7210_if = es7210_codec_new(&es7210_codec_cfg);
    ESP_RETURN_ON_FALSE(es7210_if, ESP_FAIL, TAG, "ES7210 codec new failed");

    esp_codec_dev_cfg_t mic_cfg = {
        .dev_type = ESP_CODEC_DEV_TYPE_IN,
        .codec_if = es7210_if,
        .data_if = data_if,
    };
    s_mic_dev = esp_codec_dev_new(&mic_cfg);
    ESP_RETURN_ON_FALSE(s_mic_dev, ESP_FAIL, TAG, "Mic device failed");

    esp_codec_dev_sample_info_t mic_sample = {
        .bits_per_sample = AUDIO_BITS_PER_SAMPLE,
        .channel = 2,
        .channel_mask = ESP_CODEC_DEV_MAKE_CHANNEL_MASK(0),
        .sample_rate = AUDIO_SAMPLE_RATE,
    };
    ESP_RETURN_ON_ERROR(esp_codec_dev_open(s_mic_dev, &mic_sample), TAG, "Mic open failed");
    ESP_RETURN_ON_ERROR(esp_codec_dev_set_in_gain(s_mic_dev, 36.0), TAG, "Mic gain failed");
    ESP_LOGI(TAG, "ES7210 microphone initialized via esp_codec_dev");

    // esp_codec_dev_open corrupted the shared I2S clock.
    // Fully re-initialize BOTH I2S channels with our exact config.
    i2s_channel_disable(s_tx_handle);
    i2s_channel_disable(s_rx_handle);
    i2s_del_channel(s_tx_handle);
    i2s_del_channel(s_rx_handle);
    s_tx_handle = NULL;
    s_rx_handle = NULL;

    i2s_chan_config_t chan_cfg2 = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM, I2S_ROLE_MASTER);
    chan_cfg2.auto_clear = true;
    ESP_RETURN_ON_ERROR(i2s_new_channel(&chan_cfg2, &s_tx_handle, &s_rx_handle),
                        TAG, "I2S re-create failed");

    i2s_std_config_t std_cfg2 = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(AUDIO_SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_MCK_IO,
            .bclk = I2S_BCK_IO,
            .ws = I2S_WS_IO,
            .dout = I2S_DO_IO,
            .din = I2S_DI_IO,
            .invert_flags = { .mclk_inv = false, .bclk_inv = false, .ws_inv = false },
        },
    };
    std_cfg2.clk_cfg.mclk_multiple = AUDIO_MCLK_MULTIPLE;

    ESP_RETURN_ON_ERROR(i2s_channel_init_std_mode(s_tx_handle, &std_cfg2), TAG, "I2S TX re-init failed");
    ESP_RETURN_ON_ERROR(i2s_channel_init_std_mode(s_rx_handle, &std_cfg2), TAG, "I2S RX re-init failed");
    ESP_RETURN_ON_ERROR(i2s_channel_enable(s_tx_handle), TAG, "I2S TX re-enable failed");
    ESP_RETURN_ON_ERROR(i2s_channel_enable(s_rx_handle), TAG, "I2S RX re-enable failed");
    ESP_LOGI(TAG, "I2S fully re-initialized: %dHz, MCLK=%dx", AUDIO_SAMPLE_RATE, AUDIO_MCLK_MULTIPLE);

    return ESP_OK;
}

esp_err_t audio_codecs_play_test_tone(void)
{
    if (!s_tx_handle) return ESP_ERR_INVALID_STATE;

    ESP_LOGI(TAG, "Playing 440Hz test tone for 2 seconds...");

    const int chunk_frames = 1600;
    const int chunk_bytes = chunk_frames * 4;  // stereo 16-bit = 4 bytes/frame
    const int total_chunks = 20;
    const float freq = 440.0f;
    const float amplitude = 28000.0f;

    int16_t *buf = malloc(chunk_bytes);
    if (!buf) return ESP_ERR_NO_MEM;

    for (int c = 0; c < total_chunks; c++) {
        for (int i = 0; i < chunk_frames; i++) {
            int sample_idx = c * chunk_frames + i;
            float t = (float)sample_idx / (float)AUDIO_SAMPLE_RATE;
            float angle = 2.0f * 3.14159265f * freq * t;
            float s = angle - (int)(angle / (2.0f * 3.14159265f)) * (2.0f * 3.14159265f);
            if (s > 3.14159265f) s -= 2.0f * 3.14159265f;
            float sine = (16.0f * s * (3.14159265f - (s < 0 ? -s : s))) /
                         (5.0f * 3.14159265f * 3.14159265f - 4.0f * s * (3.14159265f - (s < 0 ? -s : s)));
            int16_t val = (int16_t)(sine * amplitude);
            buf[i * 2] = val;
            buf[i * 2 + 1] = val;
        }
        size_t written = 0;
        i2s_channel_write(s_tx_handle, buf, chunk_bytes, &written, portMAX_DELAY);
    }

    free(buf);
    ESP_LOGI(TAG, "Test tone complete");
    return ESP_OK;
}

// ── Public API ────────────────────────────────────────────────

esp_err_t audio_codecs_init(void)
{
    ESP_LOGI(TAG, "Initializing audio (ES8311 speaker + ES7210 microphones)");
    ESP_RETURN_ON_ERROR(codec_init(), TAG, "Codec init failed");
    ESP_LOGI(TAG, "Audio initialized: %d Hz, %d-bit, stereo", AUDIO_SAMPLE_RATE, AUDIO_BITS_PER_SAMPLE);
    return ESP_OK;
}

esp_err_t audio_codecs_read(void *buf, size_t buf_size, size_t *bytes_read, uint32_t timeout_ms)
{
    if (!s_mic_dev) return ESP_ERR_INVALID_STATE;
    int ret = esp_codec_dev_read(s_mic_dev, buf, buf_size);
    if (ret == 0) {
        *bytes_read = buf_size;
        return ESP_OK;
    }
    *bytes_read = 0;
    return ESP_FAIL;
}

esp_err_t audio_codecs_write(const void *buf, size_t buf_size, size_t *bytes_written, uint32_t timeout_ms)
{
    if (!s_tx_handle) return ESP_ERR_INVALID_STATE;
    return i2s_channel_write(s_tx_handle, buf, buf_size, bytes_written, timeout_ms);
}

esp_err_t audio_codecs_set_volume(int percent)
{
    if (!s_es8311_dev) return ESP_ERR_INVALID_STATE;
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    // Register 0x32: 0x00=mute, 0xFF=max (+32dB)
    uint8_t vol = (uint8_t)((percent * 255) / 100);
    return es8311_write_reg(0x32, vol);
}

esp_err_t audio_codecs_set_mic_gain(int gain_db)
{
    if (!s_mic_dev) return ESP_ERR_INVALID_STATE;
    return esp_codec_dev_set_in_gain(s_mic_dev, (float)gain_db);
}
