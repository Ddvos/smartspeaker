#pragma once

#include "esp_err.h"

// Waveshare ESP32-P4 3.4" Round Display: 800x800, MIPI DSI 2-lane
#define DISPLAY_WIDTH   800
#define DISPLAY_HEIGHT  800

esp_err_t display_driver_init(void);
