#pragma once

#include "esp_err.h"
#include "config_store.h"

// TODO: Change to your SvelteKit server URL
#define API_BASE_URL    "https://192.168.2.13:5173"

typedef struct {
    char device_id[64];
    char device_token[128];
    char pairing_code[8];
    char pairing_expires_at[32];
} register_response_t;

typedef struct {
    bool paired;
    int http_status;
    char config_updated_at[32];
} heartbeat_response_t;

typedef struct {
    char code[8];
    char expires_at[32];
} pairing_code_response_t;

esp_err_t api_register_device(const char *chip_id, const char *firmware_version,
                               register_response_t *response);

esp_err_t api_send_heartbeat(const char *device_id, const char *token,
                              int wifi_rssi, const char *ip_address,
                              const char *firmware_version,
                              heartbeat_response_t *response);

esp_err_t api_get_config(const char *device_id, const char *token,
                          device_config_t *config);

esp_err_t api_refresh_pairing_code(const char *device_id, const char *token,
                                    pairing_code_response_t *response);

typedef struct {
    char gemini_token[512];
    char model[64];
    char voice[32];
    char system_prompt[1024];
    char expires_at[32];
    int speaker_volume;
    int mic_sensitivity;
} gemini_token_response_t;

esp_err_t api_get_gemini_token(const char *device_id, const char *token,
                                gemini_token_response_t *response);
