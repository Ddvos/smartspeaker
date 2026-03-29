#pragma once

#include "esp_err.h"

esp_err_t display_ui_show_boot_screen(void);
esp_err_t display_ui_show_pairing_code(const char *code);
esp_err_t display_ui_show_status(const char *device_name, bool connected, int wifi_rssi);
esp_err_t display_ui_show_wifi_connecting(void);
esp_err_t display_ui_show_error(const char *message);
