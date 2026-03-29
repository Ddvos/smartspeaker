#pragma once

#include "esp_err.h"
#include <stdbool.h>

typedef enum {
    GEMINI_STATE_IDLE,
    GEMINI_STATE_CONNECTING,
    GEMINI_STATE_SETUP,
    GEMINI_STATE_READY,
    GEMINI_STATE_LISTENING,
    GEMINI_STATE_PROCESSING,
    GEMINI_STATE_SPEAKING,
    GEMINI_STATE_ERROR,
    GEMINI_STATE_DISCONNECTED,
} gemini_state_t;

typedef struct {
    char token[512];
    char model[64];
    char voice[32];
    char system_prompt[1024];
    int speaker_volume;
    int mic_sensitivity;
} gemini_config_t;

typedef void (*gemini_state_cb_t)(gemini_state_t new_state);

esp_err_t gemini_client_init(gemini_state_cb_t state_cb);
esp_err_t gemini_client_connect(const gemini_config_t *config);
esp_err_t gemini_client_disconnect(void);
esp_err_t gemini_client_start_turn(void);
esp_err_t gemini_client_end_turn(void);
gemini_state_t gemini_client_get_state(void);
bool gemini_client_is_connected(void);
