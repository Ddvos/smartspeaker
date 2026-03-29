#pragma once

#include "esp_err.h"
#include <stdbool.h>

esp_err_t display_ui_show_boot_screen(void);
esp_err_t display_ui_show_pairing_code(const char *code);
esp_err_t display_ui_show_status(const char *device_name, bool connected, int wifi_rssi);
esp_err_t display_ui_show_wifi_connecting(void);
esp_err_t display_ui_show_error(const char *message);

// Voice interaction screens (music_cb called when play/pause tapped)
typedef void (*display_ui_music_cb_t)(void);
void display_ui_set_music_callback(display_ui_music_cb_t cb);
esp_err_t display_ui_show_voice_idle(void);
esp_err_t display_ui_show_voice_listening(void);
esp_err_t display_ui_show_voice_thinking(void);
esp_err_t display_ui_show_voice_speaking(void);
esp_err_t display_ui_show_voice_connecting(void);
