#include "audio_codecs.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

static const char *TAG = "audio_codecs";

/*
 * NOTE: The actual codec initialization requires register-level configuration
 * specific to the ES8311 and ES7210 chips. The Espressif esp-adf (Audio
 * Development Framework) provides ready-made drivers for these codecs.
 *
 * For Phase 3, we just verify I2C communication is working.
 * Full audio streaming will be implemented in a future phase.
 *
 * References:
 * - ES8311 driver: https://github.com/espressif/esp-adf/tree/master/components/audio_hal/driver/es8311
 * - ES7210 driver: https://github.com/espressif/esp-adf/tree/master/components/audio_hal/driver/es7210
 *
 * The Waveshare board connects both codecs to the same I2C bus.
 * Check the board schematic for the exact GPIO pins used for I2C SDA/SCL.
 */

// TODO: Update these pins based on Waveshare board schematic
#define I2C_SDA_PIN     8
#define I2C_SCL_PIN     9

esp_err_t audio_codecs_init(void)
{
    ESP_LOGI(TAG, "Initializing audio codecs (ES8311 + ES7210)");

    // Initialize I2C master bus
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle;
    esp_err_t err = i2c_new_master_bus(&bus_config, &bus_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create I2C master bus: %s", esp_err_to_name(err));
        return err;
    }

    // Probe ES8311 (speaker DAC)
    err = i2c_master_probe(bus_handle, ES8311_I2C_ADDR, 100);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "ES8311 (speaker DAC) detected at 0x%02X", ES8311_I2C_ADDR);
    } else {
        ESP_LOGW(TAG, "ES8311 not detected at 0x%02X (check wiring)", ES8311_I2C_ADDR);
    }

    // Probe ES7210 (microphone ADC)
    err = i2c_master_probe(bus_handle, ES7210_I2C_ADDR, 100);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "ES7210 (microphone ADC) detected at 0x%02X", ES7210_I2C_ADDR);
    } else {
        ESP_LOGW(TAG, "ES7210 not detected at 0x%02X (check wiring)", ES7210_I2C_ADDR);
    }

    // TODO: Full codec initialization (sample rate, bit depth, gain, etc.)
    // This will be done when implementing actual audio streaming.

    ESP_LOGI(TAG, "Audio codec initialization complete (probe only)");
    return ESP_OK;
}
