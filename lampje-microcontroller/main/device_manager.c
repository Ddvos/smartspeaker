#pragma GCC diagnostic ignored "-Wstringop-truncation"
#include "device_manager.h"
#include "config_store.h"
#include "api_client.h"
#include "wifi_manager.h"
#include "display_ui.h"
#include "gemini_client.h"
#include "audio_codecs.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "device_manager";
static device_state_t s_state = DEVICE_STATE_INIT;
static char s_device_id[64] = {0};
static char s_device_token[128] = {0};
static char s_last_config_updated[32] = {0};

static void get_chip_id(char *buf, size_t len)
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    snprintf(buf, len, "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

static esp_err_t do_register(void)
{
    s_state = DEVICE_STATE_REGISTERING;

    char chip_id[20];
    get_chip_id(chip_id, sizeof(chip_id));

    ESP_LOGI(TAG, "Registering device with chip ID: %s", chip_id);

    register_response_t resp = {0};
    esp_err_t err = api_register_device(chip_id, FIRMWARE_VERSION, &resp);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Registration failed");
        return err;
    }

    // Save credentials to NVS
    strncpy(s_device_id, resp.device_id, sizeof(s_device_id) - 1);
    strncpy(s_device_token, resp.device_token, sizeof(s_device_token) - 1);
    config_store_save_credentials(s_device_id, s_device_token);

    // Show pairing code on display
    s_state = DEVICE_STATE_PAIRING;
    display_ui_show_pairing_code(resp.pairing_code);

    ESP_LOGI(TAG, "Device registered, showing pairing code: %s", resp.pairing_code);
    return ESP_OK;
}

static void refresh_pairing_code(void)
{
    pairing_code_response_t resp = {0};
    esp_err_t err = api_refresh_pairing_code(s_device_id, s_device_token, &resp);
    if (err == ESP_OK) {
        display_ui_show_pairing_code(resp.code);
        ESP_LOGI(TAG, "Refreshed pairing code: %s", resp.code);
    }
}

// Returns: 1 = paired, 0 = not paired, -1 = unauthorized (credentials invalid)
static int check_if_paired(void)
{
    heartbeat_response_t resp = {0};
    char ip[16] = {0};
    wifi_manager_get_ip_str(ip, sizeof(ip));
    int rssi = wifi_manager_get_rssi();

    esp_err_t err = api_send_heartbeat(s_device_id, s_device_token,
                                        rssi, ip, FIRMWARE_VERSION, &resp);
    if (err != ESP_OK && resp.http_status == 401) {
        return -1;
    }
    return (err == ESP_OK && resp.paired) ? 1 : 0;
}

static bool s_gemini_connected = false;
static int64_t s_token_fetched_epoch = 0;

// Pending UI update (set from WS task, processed in device_manager task)
static volatile gemini_state_t s_pending_ui_state = GEMINI_STATE_IDLE;
static volatile bool s_ui_update_pending = false;

// Called from WebSocket task — do NOT call LVGL here (not thread-safe)
static void on_gemini_state_change(gemini_state_t state)
{
    s_pending_ui_state = state;
    s_ui_update_pending = true;

    // Update non-LVGL state immediately
    switch (state) {
        case GEMINI_STATE_READY:
            s_state = DEVICE_STATE_VOICE_READY;
            break;
        case GEMINI_STATE_LISTENING:
            s_state = DEVICE_STATE_VOICE_ACTIVE;
            break;
        case GEMINI_STATE_DISCONNECTED:
        case GEMINI_STATE_ERROR:
            s_state = DEVICE_STATE_RUNNING;
            s_gemini_connected = false;
            ESP_LOGW(TAG, "Gemini disconnected/error");
            break;
        default:
            break;
    }
}

// Process pending UI updates (called from device_manager task — safe for LVGL)
static void process_ui_updates(void)
{
    if (!s_ui_update_pending) return;
    s_ui_update_pending = false;

    gemini_state_t state = s_pending_ui_state;
    switch (state) {
        case GEMINI_STATE_READY:
            display_ui_show_voice_idle();
            break;
        case GEMINI_STATE_LISTENING:
            display_ui_show_voice_listening();
            break;
        case GEMINI_STATE_PROCESSING:
            display_ui_show_voice_thinking();
            break;
        case GEMINI_STATE_SPEAKING:
            display_ui_show_voice_speaking();
            break;
        default:
            break;
    }
}

static void on_voice_start(void)
{
    gemini_state_t gs = gemini_client_get_state();
    ESP_LOGI(TAG, "Start pressed, state=%d", gs);
    if (gs == GEMINI_STATE_READY) {
        gemini_client_start_turn();
    }
}

static void on_voice_stop(void)
{
    gemini_state_t gs = gemini_client_get_state();
    ESP_LOGI(TAG, "Stop pressed, state=%d", gs);
    if (gs == GEMINI_STATE_LISTENING || gs == GEMINI_STATE_SPEAKING ||
        gs == GEMINI_STATE_PROCESSING) {
        gemini_client_end_turn();
    }
}

static esp_err_t connect_gemini(void)
{
    s_state = DEVICE_STATE_VOICE_CONNECTING;
    display_ui_show_voice_connecting();

    gemini_token_response_t token_resp = {0};
    esp_err_t err = api_get_gemini_token(s_device_id, s_device_token, &token_resp);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get Gemini token");
        return err;
    }

    gemini_client_init(on_gemini_state_change);

    gemini_config_t gcfg = {0};
    strncpy(gcfg.token, token_resp.gemini_token, sizeof(gcfg.token) - 1);
    strncpy(gcfg.model, token_resp.model, sizeof(gcfg.model) - 1);
    strncpy(gcfg.voice, token_resp.voice, sizeof(gcfg.voice) - 1);
    strncpy(gcfg.system_prompt, token_resp.system_prompt, sizeof(gcfg.system_prompt) - 1);
    gcfg.speaker_volume = token_resp.speaker_volume;
    gcfg.mic_sensitivity = token_resp.mic_sensitivity;

    err = gemini_client_connect(&gcfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Gemini connection failed");
        return err;
    }

    s_gemini_connected = true;
    s_token_fetched_epoch = esp_log_timestamp() / 1000;
    ESP_LOGI(TAG, "Gemini connected, voice ready");
    return ESP_OK;
}

static void heartbeat_loop(void)
{
    s_state = DEVICE_STATE_RUNNING;

    // Fetch initial config
    device_config_t config = {0};
    if (api_get_config(s_device_id, s_device_token, &config) == ESP_OK) {
        config_store_save_config(&config);
        audio_codecs_set_volume(config.speaker_volume);
    }

    // Set voice button callbacks
    display_ui_set_voice_callbacks(on_voice_start, on_voice_stop);

    // Connect to Gemini
    if (connect_gemini() != ESP_OK) {
        ESP_LOGW(TAG, "Gemini connect failed, will retry");
        display_ui_show_status("Lampje", true, wifi_manager_get_rssi());
    }

    int heartbeat_counter = 0;
    while (1) {
        // Poll UI updates quickly (every 100ms)
        vTaskDelay(pdMS_TO_TICKS(100));
        process_ui_updates();

        // Heartbeat every 30 seconds (300 * 100ms)
        if (++heartbeat_counter < 300) {
            continue;
        }
        heartbeat_counter = 0;

        if (!wifi_manager_is_connected()) {
            continue;
        }

        char ip[16] = {0};
        wifi_manager_get_ip_str(ip, sizeof(ip));
        int rssi = wifi_manager_get_rssi();

        heartbeat_response_t resp = {0};
        esp_err_t err = api_send_heartbeat(s_device_id, s_device_token,
                                            rssi, ip, FIRMWARE_VERSION, &resp);

        if (err == ESP_OK) {
            if (resp.config_updated_at[0] != '\0' &&
                strcmp(resp.config_updated_at, s_last_config_updated) != 0) {
                ESP_LOGI(TAG, "Config changed on server, fetching...");
                device_config_t new_config = {0};
                if (api_get_config(s_device_id, s_device_token, &new_config) == ESP_OK) {
                    config_store_save_config(&new_config);
                    strncpy(s_last_config_updated, resp.config_updated_at,
                            sizeof(s_last_config_updated) - 1);
                    audio_codecs_set_volume(new_config.speaker_volume);
                }
            }
        }

        // Retry Gemini connection if disconnected
        if (!s_gemini_connected) {
            ESP_LOGI(TAG, "Retrying Gemini connection...");
            if (connect_gemini() == ESP_OK) {
                // Callbacks already set, buttons will work on next UI update
            }
        }

        // Refresh Gemini token every 25 minutes (token valid for 30 min)
        int64_t now_sec = esp_log_timestamp() / 1000;
        if (s_gemini_connected && (now_sec - s_token_fetched_epoch) > (25 * 60)) {
            ESP_LOGI(TAG, "Refreshing Gemini token...");
            gemini_client_disconnect();
            if (connect_gemini() == ESP_OK) {
                // Callbacks already set, buttons will work on next UI update
            }
        }
    }
}

static void device_manager_task(void *arg)
{
    // Step 1: Check NVS for existing credentials
    device_credentials_t creds = {0};
    config_store_get_credentials(&creds);

    if (creds.has_credentials) {
        ESP_LOGI(TAG, "Found existing credentials for device %s", creds.device_id);
        strncpy(s_device_id, creds.device_id, sizeof(s_device_id) - 1);
        strncpy(s_device_token, creds.device_token, sizeof(s_device_token) - 1);

        int paired = check_if_paired();
        if (paired == 1) {
            heartbeat_loop();  // Does not return
        } else if (paired == -1) {
            // Credentials rejected — clear and re-register
            ESP_LOGW(TAG, "Credentials rejected (401), re-registering...");
            config_store_clear();
            memset(s_device_id, 0, sizeof(s_device_id));
            memset(s_device_token, 0, sizeof(s_device_token));
            creds.has_credentials = false;
        } else {
            // Not paired yet, show pairing code
            s_state = DEVICE_STATE_PAIRING;
            refresh_pairing_code();
        }
    }

    if (!creds.has_credentials) {
        // No credentials — register as new device
        esp_err_t err = do_register();
        if (err != ESP_OK) {
            s_state = DEVICE_STATE_ERROR;
            display_ui_show_error("Registratie mislukt.\nControleer de verbinding.");
            vTaskDelete(NULL);
            return;
        }
    }

    // Wait for pairing — poll every 5 seconds
    int pairing_elapsed_ms = 0;
    while (s_state == DEVICE_STATE_PAIRING) {
        vTaskDelay(pdMS_TO_TICKS(5000));
        pairing_elapsed_ms += 5000;

        // Refresh pairing code every 9 minutes (before 10min expiry)
        if (pairing_elapsed_ms >= 9 * 60 * 1000) {
            refresh_pairing_code();
            pairing_elapsed_ms = 0;
        }

        int paired = check_if_paired();
        if (paired == 1) {
            ESP_LOGI(TAG, "Device paired! Entering heartbeat loop.");
            heartbeat_loop();  // Does not return
        } else if (paired == -1) {
            ESP_LOGW(TAG, "Credentials rejected during pairing, restarting...");
            config_store_clear();
            esp_restart();
        }
    }

    vTaskDelete(NULL);
}

esp_err_t device_manager_start(void)
{
    ESP_LOGI(TAG, "Starting device manager");

    BaseType_t ret = xTaskCreate(
        device_manager_task,
        "device_mgr",
        8192,
        NULL,
        5,
        NULL
    );

    return (ret == pdPASS) ? ESP_OK : ESP_FAIL;
}

device_state_t device_manager_get_state(void)
{
    return s_state;
}
