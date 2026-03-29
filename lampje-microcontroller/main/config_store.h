#pragma once

#include "esp_err.h"
#include <stdbool.h>

#define CONFIG_STORE_MAX_TOKEN_LEN    128
#define CONFIG_STORE_MAX_ID_LEN       64
#define CONFIG_STORE_MAX_CONFIG_LEN   1024

typedef struct {
    char device_token[CONFIG_STORE_MAX_TOKEN_LEN];
    char device_id[CONFIG_STORE_MAX_ID_LEN];
    bool has_credentials;
} device_credentials_t;

typedef struct {
    char led_idle[8];
    char led_listening[8];
    char led_thinking[8];
    char led_speaking[8];
    char led_error[8];
    int mic_sensitivity;
    int speaker_volume;
    int display_brightness;
} device_config_t;

esp_err_t config_store_init(void);
esp_err_t config_store_save_credentials(const char *device_id, const char *device_token);
esp_err_t config_store_get_credentials(device_credentials_t *creds);
esp_err_t config_store_save_config(const device_config_t *config);
esp_err_t config_store_get_config(device_config_t *config);
esp_err_t config_store_clear(void);
