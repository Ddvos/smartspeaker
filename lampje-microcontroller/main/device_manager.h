#pragma once

#include "esp_err.h"

#define FIRMWARE_VERSION    "0.1.0"
#define HEARTBEAT_INTERVAL_MS   30000   // 30 seconds

typedef enum {
    DEVICE_STATE_INIT,
    DEVICE_STATE_REGISTERING,
    DEVICE_STATE_PAIRING,
    DEVICE_STATE_RUNNING,
    DEVICE_STATE_ERROR
} device_state_t;

esp_err_t device_manager_start(void);
device_state_t device_manager_get_state(void);
