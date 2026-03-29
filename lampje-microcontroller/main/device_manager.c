#include "device_manager.h"
#include "config_store.h"
#include "api_client.h"
#include "wifi_manager.h"
#include "display_ui.h"
#include "esp_log.h"
#include "esp_mac.h"
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

static bool check_if_paired(void)
{
    // Send a heartbeat — if the device has been paired, the server will accept it
    // and we can transition to the running state
    heartbeat_response_t resp = {0};
    char ip[16] = {0};
    wifi_manager_get_ip_str(ip, sizeof(ip));
    int rssi = wifi_manager_get_rssi();

    esp_err_t err = api_send_heartbeat(s_device_id, s_device_token,
                                        rssi, ip, FIRMWARE_VERSION, &resp);
    return (err == ESP_OK);
}

static void heartbeat_loop(void)
{
    s_state = DEVICE_STATE_RUNNING;

    // Fetch initial config
    device_config_t config = {0};
    if (api_get_config(s_device_id, s_device_token, &config) == ESP_OK) {
        config_store_save_config(&config);
        // TODO: Apply config (LED colors, volume, brightness)
    }

    // Show status screen
    display_ui_show_status("Lampje", true, wifi_manager_get_rssi());

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(HEARTBEAT_INTERVAL_MS));

        if (!wifi_manager_is_connected()) {
            display_ui_show_status("Lampje", false, 0);
            continue;
        }

        char ip[16] = {0};
        wifi_manager_get_ip_str(ip, sizeof(ip));
        int rssi = wifi_manager_get_rssi();

        heartbeat_response_t resp = {0};
        esp_err_t err = api_send_heartbeat(s_device_id, s_device_token,
                                            rssi, ip, FIRMWARE_VERSION, &resp);

        if (err == ESP_OK) {
            // Check if config was updated on server
            if (resp.config_updated_at[0] != '\0' &&
                strcmp(resp.config_updated_at, s_last_config_updated) != 0) {
                ESP_LOGI(TAG, "Config changed on server, fetching...");
                device_config_t new_config = {0};
                if (api_get_config(s_device_id, s_device_token, &new_config) == ESP_OK) {
                    config_store_save_config(&new_config);
                    strncpy(s_last_config_updated, resp.config_updated_at,
                            sizeof(s_last_config_updated) - 1);
                    // TODO: Apply new config to hardware
                }
            }

            display_ui_show_status("Lampje", true, rssi);
        } else {
            ESP_LOGW(TAG, "Heartbeat failed");
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

        // Try a heartbeat to see if we're paired
        if (check_if_paired()) {
            heartbeat_loop();  // Does not return
        }

        // Not paired yet, show pairing code
        s_state = DEVICE_STATE_PAIRING;
        refresh_pairing_code();
    } else {
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

        if (check_if_paired()) {
            ESP_LOGI(TAG, "Device paired! Entering heartbeat loop.");
            heartbeat_loop();  // Does not return
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
