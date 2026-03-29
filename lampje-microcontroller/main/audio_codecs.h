#pragma once

#include "esp_err.h"

// ES8311 (DAC/speaker) I2C address
#define ES8311_I2C_ADDR     0x18

// ES7210 (ADC/microphones) I2C address
#define ES7210_I2C_ADDR     0x40

esp_err_t audio_codecs_init(void);
