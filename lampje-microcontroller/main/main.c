#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "config_store.h"
#include "wifi_manager.h"
#include "display_driver.h"
#include "display_ui.h"
#include "audio_codecs.h"
#include "device_manager.h"

static const char *TAG = "lampje";

void app_main(void)
{
    ESP_LOGI(TAG, "=== Lampje Firmware v%s ===", FIRMWARE_VERSION);

    // 1. Initialize NVS (persistent config storage)
    ESP_ERROR_CHECK(config_store_init());

    // 2. Initialize display
    ESP_ERROR_CHECK(display_driver_init());
    display_ui_show_boot_screen();

    // Give the user a moment to see the boot screen
    vTaskDelay(pdMS_TO_TICKS(1000));

    // 3. Connect to WiFi
    display_ui_show_wifi_connecting();
    esp_err_t wifi_err = wifi_manager_init();
    if (wifi_err != ESP_OK) {
        display_ui_show_error("WiFi verbinding mislukt.\nControleer je instellingen.");
        ESP_LOGE(TAG, "WiFi failed, halting.");
        return;
    }

    // 4. Initialize audio codecs (ES8311 speaker + ES7210 microphones)
    esp_err_t audio_err = audio_codecs_init();
    if (audio_err != ESP_OK) {
        ESP_LOGW(TAG, "Audio init failed: %s (continuing without audio)", esp_err_to_name(audio_err));
    }

    // 5. Start device manager (registration, pairing, heartbeat loop)
    ESP_ERROR_CHECK(device_manager_start());

    // LVGL rendering runs in its own task (started by display_driver_init)
    // Keep main task alive
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
