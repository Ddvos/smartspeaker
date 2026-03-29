#pragma once

#include "esp_err.h"
#include <stdbool.h>

// TODO: Move to NVS or provisioning screen in future
#define WIFI_SSID       "KPND39256"
#define WIFI_PASSWORD   "RPd4CxzvxWspVjLt"

esp_err_t wifi_manager_init(void);
bool wifi_manager_is_connected(void);
int wifi_manager_get_rssi(void);
esp_err_t wifi_manager_get_ip_str(char *buf, size_t buf_len);
