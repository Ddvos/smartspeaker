#pragma once

#include "esp_err.h"
#include <stdint.h>
#include <stddef.h>

// I2C addresses
#define ES8311_I2C_ADDR     0x18
#define ES7210_I2C_ADDR     0x40

// Audio configuration
#define AUDIO_SAMPLE_RATE       16000
#define AUDIO_BITS_PER_SAMPLE   16
#define AUDIO_MCLK_MULTIPLE     384

// Buffer for read/write operations (~150ms at 16kHz stereo 16-bit)
#define AUDIO_CHUNK_BYTES       9600

// Initialize audio hardware: I2C, ES8311 codec, I2S TX+RX, PA enable
esp_err_t audio_codecs_init(void);

// Read captured audio from microphone (blocks until data available)
// Returns ESP_OK on success, buf filled with interleaved stereo 16-bit PCM
esp_err_t audio_codecs_read(void *buf, size_t buf_size, size_t *bytes_read, uint32_t timeout_ms);

// Write audio for playback through speaker (blocks until buffer space available)
// buf should contain interleaved stereo 16-bit PCM at AUDIO_SAMPLE_RATE
esp_err_t audio_codecs_write(const void *buf, size_t buf_size, size_t *bytes_written, uint32_t timeout_ms);

// Set speaker volume (0-100)
esp_err_t audio_codecs_set_volume(int percent);

// Set microphone gain (ES8311_MIC_GAIN_0DB .. ES8311_MIC_GAIN_42_5DB)
esp_err_t audio_codecs_set_mic_gain(int gain_db);

// Play a 440Hz test tone for ~2 seconds to verify speaker works
esp_err_t audio_codecs_play_test_tone(void);

// Play/stop embedded test music (canon.pcm) in background
void audio_codecs_play_music(void);
void audio_codecs_stop_music(void);
bool audio_codecs_is_playing_music(void);
