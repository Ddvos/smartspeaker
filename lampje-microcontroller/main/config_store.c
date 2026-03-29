#include "config_store.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "config_store";
static const char *NVS_NAMESPACE = "lampje";

esp_err_t config_store_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition needs erase, erasing...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    return err;
}

esp_err_t config_store_save_credentials(const char *device_id, const char *device_token)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_str(handle, "device_id", device_id);
    if (err == ESP_OK) {
        err = nvs_set_str(handle, "device_token", device_token);
    }
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }

    nvs_close(handle);
    ESP_LOGI(TAG, "Credentials saved for device %s", device_id);
    return err;
}

esp_err_t config_store_get_credentials(device_credentials_t *creds)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        creds->has_credentials = false;
        return err;
    }

    size_t id_len = CONFIG_STORE_MAX_ID_LEN;
    size_t token_len = CONFIG_STORE_MAX_TOKEN_LEN;

    err = nvs_get_str(handle, "device_id", creds->device_id, &id_len);
    if (err == ESP_OK) {
        err = nvs_get_str(handle, "device_token", creds->device_token, &token_len);
    }

    creds->has_credentials = (err == ESP_OK);
    nvs_close(handle);
    return err;
}

esp_err_t config_store_save_config(const device_config_t *config)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_blob(handle, "hw_config", config, sizeof(device_config_t));
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }

    nvs_close(handle);
    return err;
}

esp_err_t config_store_get_config(device_config_t *config)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    size_t len = sizeof(device_config_t);
    err = nvs_get_blob(handle, "hw_config", config, &len);

    nvs_close(handle);

    // Set defaults if not found
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        strncpy(config->led_idle, "#81ecff", sizeof(config->led_idle));
        strncpy(config->led_listening, "#00d4ec", sizeof(config->led_listening));
        strncpy(config->led_thinking, "#a68cff", sizeof(config->led_thinking));
        strncpy(config->led_speaking, "#ff7520", sizeof(config->led_speaking));
        strncpy(config->led_error, "#ff716c", sizeof(config->led_error));
        config->mic_sensitivity = 70;
        config->speaker_volume = 65;
        config->display_brightness = 80;
        return ESP_OK;
    }

    return err;
}

esp_err_t config_store_clear(void)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_erase_all(handle);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }

    nvs_close(handle);
    return err;
}
