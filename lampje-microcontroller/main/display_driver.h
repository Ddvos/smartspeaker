#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"

// Waveshare ESP32-P4 3.4" Round Display: 800x800, MIPI DSI 2-lane
#define DISPLAY_WIDTH   800
#define DISPLAY_HEIGHT  800

esp_err_t display_driver_init(void);

// Get the shared I2C bus handle (initialized by display_driver for touch)
i2c_master_bus_handle_t display_driver_get_i2c_bus(void);
