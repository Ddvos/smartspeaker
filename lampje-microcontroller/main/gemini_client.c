#include "gemini_client.h"
#include "audio_codecs.h"
#include "esp_websocket_client.h"
#include "esp_log.h"
#include "esp_event.h"
#include "cJSON.h"
#include "mbedtls/base64.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/ringbuf.h"
#include "esp_heap_caps.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "gemini_client";

// WebSocket URL for Gemini Live API (v1alpha)
#define GEMINI_WS_URL "wss://generativelanguage.googleapis.com/ws/google.ai.generativelanguage.v1alpha.GenerativeService.BidiGenerateContent"

// Event group bits
#define BIT_CONNECTED       BIT0
#define BIT_SETUP_COMPLETE  BIT1
#define BIT_TURN_ACTIVE     BIT2
#define BIT_RESPONSE_ACTIVE BIT3
#define BIT_DISCONNECT      BIT4

// Audio chunk: ~100ms of mono 16kHz 16-bit = 1600 samples = 3200 bytes
// I2S is stereo, so we read 6400 bytes and extract left channel
#define MONO_CHUNK_SAMPLES  1600
#define MONO_CHUNK_BYTES    (MONO_CHUNK_SAMPLES * 2)
#define STEREO_CHUNK_BYTES  (MONO_CHUNK_SAMPLES * 4)

// Base64 output for 3200 bytes = ceil(3200/3)*4 = 4268 bytes
#define B64_CHUNK_SIZE      4300

// Max size for JSON send message: JSON overhead + base64 data
#define SEND_BUF_SIZE       (B64_CHUNK_SIZE + 256)

// Receive buffer for incoming WebSocket frames (must hold largest Gemini message)
#define RECV_BUF_SIZE       65536

static esp_websocket_client_handle_t s_ws_client = NULL;
static EventGroupHandle_t s_events = NULL;
static gemini_state_t s_state = GEMINI_STATE_IDLE;
static gemini_state_cb_t s_state_cb = NULL;
static gemini_config_t s_config = {0};
static TaskHandle_t s_send_task = NULL;
static volatile bool s_conversation_active = false;

// Playback ring buffer (audio decoded in WS handler, played in separate task)
// Ring buffer for audio playback (needs to hold full Gemini response)
// Gemini sends all audio at once; playback is real-time. Need large buffer.
#define PLAYBACK_BUF_SIZE   (2 * 1024 * 1024)  // 2MB from PSRAM (32MB available)
static RingbufHandle_t s_play_ringbuf = NULL;
static TaskHandle_t s_play_task = NULL;

// Receive accumulation buffer (WebSocket frames may be fragmented)
static char *s_recv_buf = NULL;
static int s_recv_len = 0;

static void set_state(gemini_state_t new_state)
{
    if (s_state != new_state) {
        s_state = new_state;
        ESP_LOGI(TAG, "State -> %d", new_state);
        if (s_state_cb) {
            s_state_cb(new_state);
        }
    }
}


static void stereo_to_mono(const int16_t *stereo, int16_t *mono, int mono_samples)
{
    for (int i = 0; i < mono_samples; i++) {
        mono[i] = stereo[i * 2];
    }
}

static void send_setup_message(void)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *setup = cJSON_AddObjectToObject(root, "setup");

    char model_str[80];
    snprintf(model_str, sizeof(model_str), "models/%s", s_config.model);
    cJSON_AddStringToObject(setup, "model", model_str);

    cJSON *gen_config = cJSON_AddObjectToObject(setup, "generationConfig");
    cJSON *modalities = cJSON_AddArrayToObject(gen_config, "responseModalities");
    cJSON_AddItemToArray(modalities, cJSON_CreateString("AUDIO"));

    cJSON *speech = cJSON_AddObjectToObject(gen_config, "speechConfig");
    cJSON *voice_cfg = cJSON_AddObjectToObject(speech, "voiceConfig");
    cJSON *prebuilt = cJSON_AddObjectToObject(voice_cfg, "prebuiltVoiceConfig");
    cJSON_AddStringToObject(prebuilt, "voiceName", s_config.voice);

    if (s_config.system_prompt[0] != '\0') {
        cJSON *sys = cJSON_AddObjectToObject(setup, "systemInstruction");
        cJSON *parts = cJSON_AddArrayToObject(sys, "parts");
        cJSON *part = cJSON_CreateObject();
        cJSON_AddStringToObject(part, "text", s_config.system_prompt);
        cJSON_AddItemToArray(parts, part);
    }

    char *msg = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (msg) {
        ESP_LOGI(TAG, "Sending setup (%d bytes): %.200s", (int)strlen(msg), msg);
        esp_websocket_client_send_text(s_ws_client, msg, strlen(msg), portMAX_DELAY);
        free(msg);
    }

    set_state(GEMINI_STATE_SETUP);
}

static void handle_received_message(const char *data, int len)
{
    // Log first 300 chars of every received message for debugging
    ESP_LOGI(TAG, "Received (%d bytes): %.300s", len, data);

    cJSON *json = cJSON_Parse(data);
    if (!json) {
        ESP_LOGW(TAG, "Failed to parse received JSON");
        return;
    }

    // Check for error
    cJSON *error = cJSON_GetObjectItem(json, "error");
    if (error) {
        cJSON *msg = cJSON_GetObjectItem(error, "message");
        ESP_LOGE(TAG, "Gemini error: %s", msg ? msg->valuestring : "(no message)");
        cJSON_Delete(json);
        set_state(GEMINI_STATE_ERROR);
        return;
    }

    // Check for setupComplete
    cJSON *setup_complete = cJSON_GetObjectItem(json, "setupComplete");
    if (setup_complete) {
        ESP_LOGI(TAG, "Setup complete");
        xEventGroupSetBits(s_events, BIT_SETUP_COMPLETE);
        set_state(GEMINI_STATE_READY);
        cJSON_Delete(json);
        return;
    }

    // Check for serverContent (audio response or turn complete)
    cJSON *server_content = cJSON_GetObjectItem(json, "serverContent");
    if (server_content) {
        // Check if user interrupted the model
        cJSON *interrupted = cJSON_GetObjectItem(server_content, "interrupted");
        if (interrupted && cJSON_IsTrue(interrupted)) {
            ESP_LOGI(TAG, "User interrupted Gemini - clearing playback");
            xEventGroupClearBits(s_events, BIT_RESPONSE_ACTIVE);
            // Flush the playback ring buffer
            if (s_play_ringbuf) {
                void *item;
                size_t sz;
                while ((item = xRingbufferReceive(s_play_ringbuf, &sz, 0)) != NULL) {
                    vRingbufferReturnItem(s_play_ringbuf, item);
                }
            }
            set_state(GEMINI_STATE_LISTENING);
            cJSON_Delete(json);
            return;
        }

        // Check turnComplete
        cJSON *turn_complete = cJSON_GetObjectItem(server_content, "turnComplete");
        if (turn_complete && cJSON_IsTrue(turn_complete)) {
            ESP_LOGI(TAG, "Turn complete");
            xEventGroupClearBits(s_events, BIT_RESPONSE_ACTIVE);
            if (s_conversation_active) {
                // Auto-resume listening for continuous conversation
                ESP_LOGI(TAG, "Auto-resuming listening");
                xEventGroupSetBits(s_events, BIT_TURN_ACTIVE);
                set_state(GEMINI_STATE_LISTENING);
            } else {
                set_state(GEMINI_STATE_READY);
            }
            cJSON_Delete(json);
            return;
        }

        // Check for model audio output
        cJSON *model_turn = cJSON_GetObjectItem(server_content, "modelTurn");
        if (model_turn) {
            // Ignore audio if conversation was stopped by user
            if (!s_conversation_active && s_state == GEMINI_STATE_READY) {
                cJSON_Delete(json);
                return;
            }
            if (s_state != GEMINI_STATE_SPEAKING) {
                // Keep mic streaming so user can interrupt (barge-in)
                // Gemini VAD will detect user speaking and cancel its response
                set_state(GEMINI_STATE_SPEAKING);
                xEventGroupSetBits(s_events, BIT_RESPONSE_ACTIVE);
                ESP_LOGI(TAG, "Gemini speaking (mic still active for interruption)");
            }

            cJSON *parts = cJSON_GetObjectItem(model_turn, "parts");
            if (parts && cJSON_IsArray(parts)) {
                cJSON *part;
                cJSON_ArrayForEach(part, parts) {
                    cJSON *inline_data = cJSON_GetObjectItem(part, "inlineData");
                    if (inline_data) {
                        cJSON *b64_data = cJSON_GetObjectItem(inline_data, "data");
                        if (b64_data && b64_data->valuestring) {
                            // Decode base64 audio
                            size_t b64_len = strlen(b64_data->valuestring);
                            size_t decoded_len = 0;
                            // Max decoded size = 3/4 of base64 length
                            uint8_t *pcm_buf = malloc((b64_len * 3) / 4 + 4);
                            if (pcm_buf) {
                                int ret = mbedtls_base64_decode(pcm_buf, (b64_len * 3) / 4 + 4,
                                                                &decoded_len,
                                                                (const uint8_t *)b64_data->valuestring,
                                                                b64_len);
                                if (ret == 0 && decoded_len > 0 && s_play_ringbuf) {
                                    // Gemini outputs 24kHz mono, resample to 16kHz stereo
                                    // Ratio 24k/16k = 3/2: for every 3 input samples, output 2
                                    int in_samples = decoded_len / 2;
                                    int out_samples = (in_samples * 2 + 2) / 3;
                                    int16_t *in_pcm = (int16_t *)pcm_buf;

                                    int16_t *out_buf = malloc(out_samples * 4);
                                    if (out_buf) {
                                        for (int i = 0; i < out_samples; i++) {
                                            // Source position = i * 1.5 (fixed point: i*3/2)
                                            int idx = (i * 3) / 2;
                                            int frac = (i * 3) % 2;

                                            int16_t sample;
                                            if (idx >= in_samples - 1) {
                                                sample = in_pcm[in_samples - 1];
                                            } else if (frac == 0) {
                                                sample = in_pcm[idx];
                                            } else {
                                                sample = (int16_t)(((int32_t)in_pcm[idx] + (int32_t)in_pcm[idx + 1]) / 2);
                                            }
                                            // Duplicate to stereo
                                            out_buf[i * 2] = sample;
                                            out_buf[i * 2 + 1] = sample;
                                        }
                                        if (!xRingbufferSend(s_play_ringbuf, out_buf,
                                                       out_samples * 4, 0)) {
                                            ESP_LOGW(TAG, "Playback buffer full, dropped %d bytes", out_samples * 4);
                                        }
                                        free(out_buf);
                                    }
                                }
                                free(pcm_buf);
                            }
                        }
                    }
                }
            }
        }
    }

    cJSON_Delete(json);
}

static void ws_event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data)
{
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;

    switch (event_id) {
        case WEBSOCKET_EVENT_CONNECTED:
            ESP_LOGI(TAG, "WebSocket connected");
            xEventGroupSetBits(s_events, BIT_CONNECTED);
            xEventGroupClearBits(s_events, BIT_DISCONNECT);
            send_setup_message();
            break;

        case WEBSOCKET_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "WebSocket disconnected");
            xEventGroupSetBits(s_events, BIT_DISCONNECT);
            xEventGroupClearBits(s_events, BIT_CONNECTED | BIT_SETUP_COMPLETE |
                                 BIT_TURN_ACTIVE | BIT_RESPONSE_ACTIVE);
            set_state(GEMINI_STATE_DISCONNECTED);
            break;

        case WEBSOCKET_EVENT_DATA:
            ESP_LOGI(TAG, "WS data: opcode=%d, len=%d, offset=%d, total=%d",
                     data->op_code, data->data_len,
                     (int)data->payload_offset, (int)data->payload_len);

            if (data->op_code == 0x01 || data->op_code == 0x00 || data->op_code == 0x02) {
                // Text frame, continuation, or binary frame (Gemini sends binary)
                if (s_recv_buf) {
                    // Accumulate data
                    if (s_recv_len + data->data_len < RECV_BUF_SIZE - 1) {
                        memcpy(s_recv_buf + s_recv_len, data->data_ptr, data->data_len);
                        s_recv_len += data->data_len;
                        s_recv_buf[s_recv_len] = '\0';
                    } else {
                        ESP_LOGW(TAG, "Recv buffer overflow, dropping data");
                        s_recv_len = 0;
                    }

                    // If this is the final fragment, process the complete message
                    if (data->data_len + data->payload_offset >= data->payload_len) {
                        handle_received_message(s_recv_buf, s_recv_len);
                        s_recv_len = 0;
                    }
                }
            } else if (data->op_code == 0x09) {
                // Ping - handled automatically by esp_websocket_client
                ESP_LOGD(TAG, "Ping received");
            } else if (data->op_code == 0x08) {
                // Close frame: first 2 bytes = status code, rest = UTF-8 reason
                if (data->data_len >= 2) {
                    uint16_t code = ((uint8_t)data->data_ptr[0] << 8) | (uint8_t)data->data_ptr[1];
                    char reason[256] = {0};
                    int reason_len = data->data_len - 2;
                    if (reason_len > 0 && reason_len < (int)sizeof(reason)) {
                        memcpy(reason, data->data_ptr + 2, reason_len);
                        reason[reason_len] = '\0';
                    }
                    ESP_LOGE(TAG, "Close frame: code=%d, reason=%s", code, reason);
                } else {
                    ESP_LOGW(TAG, "Close frame received (no reason)");
                }
            }
            break;

        case WEBSOCKET_EVENT_ERROR:
            ESP_LOGE(TAG, "WebSocket error");
            set_state(GEMINI_STATE_ERROR);
            break;

        default:
            break;
    }
}

static void audio_play_task(void *arg)
{
    ESP_LOGI(TAG, "Playback task started");
    static int play_log_cnt = 0;
    while (1) {
        size_t item_size = 0;
        void *item = xRingbufferReceiveUpTo(s_play_ringbuf, &item_size, pdMS_TO_TICKS(200), 4096);
        if (item && item_size > 0) {
            if (play_log_cnt++ % 20 == 0) {
                ESP_LOGI(TAG, "Playing %d bytes", (int)item_size);
            }
            size_t bytes_written = 0;
            audio_codecs_write(item, item_size, &bytes_written, portMAX_DELAY);
            vRingbufferReturnItem(s_play_ringbuf, item);
        }
    }
    vTaskDelete(NULL);
}

static void audio_send_task(void *arg)
{
    // Buffers for audio capture and encoding
    uint8_t *stereo_buf = malloc(STEREO_CHUNK_BYTES);
    int16_t *mono_buf = malloc(MONO_CHUNK_BYTES);
    char *b64_buf = malloc(B64_CHUNK_SIZE);
    char *send_buf = malloc(SEND_BUF_SIZE);

    if (!stereo_buf || !mono_buf || !b64_buf || !send_buf) {
        ESP_LOGE(TAG, "Failed to allocate send task buffers");
        goto cleanup;
    }

    while (1) {
        // Wait for turn to be active
        xEventGroupWaitBits(s_events, BIT_TURN_ACTIVE, false, true, portMAX_DELAY);

        // Check if we should stop
        if (xEventGroupGetBits(s_events) & BIT_DISCONNECT) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        // Read stereo audio from microphone
        size_t bytes_read = 0;
        esp_err_t err = audio_codecs_read(stereo_buf, STEREO_CHUNK_BYTES, &bytes_read, 200);
        if (err != ESP_OK || bytes_read == 0) {
            continue;
        }

        // Convert stereo to mono (left channel)
        int stereo_samples = bytes_read / 4;
        if (stereo_samples > MONO_CHUNK_SAMPLES) stereo_samples = MONO_CHUNK_SAMPLES;
        stereo_to_mono((const int16_t *)stereo_buf, mono_buf, stereo_samples);

        // Base64 encode the mono PCM
        size_t b64_len = 0;
        int ret = mbedtls_base64_encode((uint8_t *)b64_buf, B64_CHUNK_SIZE, &b64_len,
                                         (const uint8_t *)mono_buf, stereo_samples * 2);
        if (ret != 0) {
            ESP_LOGW(TAG, "Base64 encode failed: %d", ret);
            continue;
        }
        b64_buf[b64_len] = '\0';

        // Debug: log audio level every ~1s
        static int audio_log_cnt = 0;
        if (audio_log_cnt++ % 10 == 0) {
            int16_t peak = 0;
            for (int i = 0; i < stereo_samples; i++) {
                int16_t v = mono_buf[i] < 0 ? -mono_buf[i] : mono_buf[i];
                if (v > peak) peak = v;
            }
            ESP_LOGI(TAG, "Audio: peak=%d samples=%d", peak, stereo_samples);
        }

        // Build JSON message using snprintf for performance (hot path)
        int msg_len = snprintf(send_buf, SEND_BUF_SIZE,
            "{\"realtimeInput\":{\"audio\":{\"data\":\"%s\",\"mimeType\":\"audio/pcm;rate=16000\"}}}",
            b64_buf);

        if (msg_len > 0 && msg_len < SEND_BUF_SIZE && s_ws_client &&
            esp_websocket_client_is_connected(s_ws_client)) {
            esp_websocket_client_send_text(s_ws_client, send_buf, msg_len, 100);
        }
    }

cleanup:
    free(stereo_buf);
    free(mono_buf);
    free(b64_buf);
    free(send_buf);
    vTaskDelete(NULL);
}

esp_err_t gemini_client_init(gemini_state_cb_t state_cb)
{
    s_state_cb = state_cb;

    if (!s_events) {
        s_events = xEventGroupCreate();
        if (!s_events) return ESP_ERR_NO_MEM;
    }

    if (!s_recv_buf) {
        s_recv_buf = heap_caps_malloc(RECV_BUF_SIZE, MALLOC_CAP_SPIRAM);
        if (!s_recv_buf) return ESP_ERR_NO_MEM;
    }
    s_recv_len = 0;

    set_state(GEMINI_STATE_IDLE);
    return ESP_OK;
}

esp_err_t gemini_client_connect(const gemini_config_t *config)
{
    if (s_ws_client) {
        gemini_client_disconnect();
    }

    memcpy(&s_config, config, sizeof(gemini_config_t));

    // Build WebSocket URL with API key
    char *url = malloc(1024);
    if (!url) return ESP_ERR_NO_MEM;
    snprintf(url, 1024, "%s?key=%s", GEMINI_WS_URL, config->token);

    set_state(GEMINI_STATE_CONNECTING);

    esp_websocket_client_config_t ws_cfg = {
        .uri = url,
        .buffer_size = RECV_BUF_SIZE,
        .task_stack = 8192,
    };

    s_ws_client = esp_websocket_client_init(&ws_cfg);
    free(url);

    if (!s_ws_client) {
        set_state(GEMINI_STATE_ERROR);
        return ESP_FAIL;
    }

    esp_websocket_register_events(s_ws_client, WEBSOCKET_EVENT_ANY, ws_event_handler, NULL);
    esp_err_t err = esp_websocket_client_start(s_ws_client);
    if (err != ESP_OK) {
        set_state(GEMINI_STATE_ERROR);
        return err;
    }

    // Wait for connection + setup complete (up to 15 seconds)
    EventBits_t bits = xEventGroupWaitBits(s_events,
        BIT_SETUP_COMPLETE | BIT_DISCONNECT,
        false, false, pdMS_TO_TICKS(15000));

    if (bits & BIT_DISCONNECT) {
        ESP_LOGE(TAG, "Connection failed");
        set_state(GEMINI_STATE_ERROR);
        return ESP_FAIL;
    }

    if (!(bits & BIT_SETUP_COMPLETE)) {
        ESP_LOGE(TAG, "Setup timeout");
        gemini_client_disconnect();
        return ESP_ERR_TIMEOUT;
    }

    // Create playback ring buffer (storage in PSRAM, control struct in internal RAM)
    if (!s_play_ringbuf) {
        StaticRingbuffer_t *rb_struct = calloc(1, sizeof(StaticRingbuffer_t));
        uint8_t *rb_storage = heap_caps_calloc(1, PLAYBACK_BUF_SIZE, MALLOC_CAP_SPIRAM);
        if (rb_struct && rb_storage) {
            s_play_ringbuf = xRingbufferCreateStatic(PLAYBACK_BUF_SIZE, RINGBUF_TYPE_BYTEBUF,
                                                      rb_storage, rb_struct);
        }
        if (!s_play_ringbuf) {
            ESP_LOGE(TAG, "Failed to create playback ring buffer!");
            free(rb_struct);
            heap_caps_free(rb_storage);
        } else {
            ESP_LOGI(TAG, "Playback ring buffer created (%d KB in PSRAM)", PLAYBACK_BUF_SIZE / 1024);
        }
    }
    if (!s_play_task) {
        xTaskCreate(audio_play_task, "gemini_play", 4096, NULL, 6, &s_play_task);
    }

    // Start audio send task
    xTaskCreate(audio_send_task, "gemini_send", 8192, NULL, 6, &s_send_task);

    // Apply audio settings
    audio_codecs_set_volume(config->speaker_volume);

    ESP_LOGI(TAG, "Connected to Gemini Live API");
    return ESP_OK;
}

esp_err_t gemini_client_disconnect(void)
{
    xEventGroupSetBits(s_events, BIT_DISCONNECT);
    xEventGroupClearBits(s_events, BIT_CONNECTED | BIT_SETUP_COMPLETE |
                         BIT_TURN_ACTIVE | BIT_RESPONSE_ACTIVE);

    if (s_send_task) {
        vTaskDelay(pdMS_TO_TICKS(200));
        vTaskDelete(s_send_task);
        s_send_task = NULL;
    }

    if (s_ws_client) {
        esp_websocket_client_stop(s_ws_client);
        esp_websocket_client_destroy(s_ws_client);
        s_ws_client = NULL;
    }

    s_recv_len = 0;
    set_state(GEMINI_STATE_IDLE);
    return ESP_OK;
}

esp_err_t gemini_client_start_turn(void)
{
    if (s_state != GEMINI_STATE_READY) {
        ESP_LOGW(TAG, "Cannot start turn in state %d", s_state);
        return ESP_ERR_INVALID_STATE;
    }

    s_conversation_active = true;
    xEventGroupSetBits(s_events, BIT_TURN_ACTIVE);
    set_state(GEMINI_STATE_LISTENING);
    ESP_LOGI(TAG, "Conversation started - listening");
    return ESP_OK;
}

esp_err_t gemini_client_end_turn(void)
{
    // Stop conversation mode entirely
    s_conversation_active = false;
    xEventGroupClearBits(s_events, BIT_TURN_ACTIVE);
    set_state(GEMINI_STATE_READY);
    ESP_LOGI(TAG, "Conversation stopped");

    return ESP_OK;
}

gemini_state_t gemini_client_get_state(void)
{
    return s_state;
}

bool gemini_client_is_connected(void)
{
    return s_state >= GEMINI_STATE_READY && s_state <= GEMINI_STATE_SPEAKING;
}
