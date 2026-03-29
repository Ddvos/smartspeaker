#include "display_ui.h"
#include "display_driver.h"
#include "lvgl.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "display_ui";

// Colors matching the Lampje brand
#define COLOR_BG        lv_color_hex(0x0f0f14)
#define COLOR_SURFACE   lv_color_hex(0x1a1a24)
#define COLOR_PRIMARY   lv_color_hex(0x81ecff)
#define COLOR_TEXT       lv_color_hex(0xf0f0f0)
#define COLOR_TEXT_DIM   lv_color_hex(0x8888a0)
#define COLOR_ERROR      lv_color_hex(0xff716c)

static lv_obj_t *s_current_screen = NULL;

static void clear_screen(void)
{
    if (s_current_screen) {
        lv_obj_del(s_current_screen);
    }
    s_current_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(s_current_screen, COLOR_BG, 0);
    lv_obj_set_style_bg_opa(s_current_screen, LV_OPA_COVER, 0);
    lv_screen_load(s_current_screen);
}

esp_err_t display_ui_show_boot_screen(void)
{
    ESP_LOGI(TAG, "Showing boot screen");
    clear_screen();

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, "lampje");
    lv_obj_set_style_text_color(label, COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_48, 0);
    lv_obj_center(label);

    lv_obj_t *sub = lv_label_create(s_current_screen);
    lv_label_set_text(sub, "Starting...");
    lv_obj_set_style_text_color(sub, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_font(sub, &lv_font_montserrat_16, 0);
    lv_obj_align(sub, LV_ALIGN_CENTER, 0, 40);

    return ESP_OK;
}

esp_err_t display_ui_show_wifi_connecting(void)
{
    ESP_LOGI(TAG, "Showing WiFi connecting screen");
    clear_screen();

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, "WiFi verbinden...");
    lv_obj_set_style_text_color(label, COLOR_TEXT, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_center(label);

    // Spinner
    lv_obj_t *spinner = lv_spinner_create(s_current_screen);
    lv_obj_set_size(spinner, 60, 60);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, -60);
    lv_spinner_set_anim_params(spinner, 1000, 200);

    return ESP_OK;
}

esp_err_t display_ui_show_pairing_code(const char *code)
{
    ESP_LOGI(TAG, "Showing pairing code: %s", code);
    clear_screen();

    // Title
    lv_obj_t *title = lv_label_create(s_current_screen);
    lv_label_set_text(title, "Koppelcode");
    lv_obj_set_style_text_color(title, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -100);

    // Display each digit with spacing
    char spaced_code[32];
    int j = 0;
    for (int i = 0; code[i] && i < 6; i++) {
        if (i > 0) spaced_code[j++] = ' ';
        if (i == 3) spaced_code[j++] = ' ';
        spaced_code[j++] = code[i];
    }
    spaced_code[j] = '\0';

    lv_obj_t *code_label = lv_label_create(s_current_screen);
    lv_label_set_text(code_label, spaced_code);
    lv_obj_set_style_text_color(code_label, COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(code_label, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_letter_space(code_label, 8, 0);
    lv_obj_center(code_label);

    // Instruction
    lv_obj_t *instruction = lv_label_create(s_current_screen);
    lv_label_set_text(instruction, "Voer deze code in\nop lampje.app");
    lv_obj_set_style_text_color(instruction, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_font(instruction, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(instruction, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(instruction, LV_ALIGN_CENTER, 0, 80);

    return ESP_OK;
}

esp_err_t display_ui_show_status(const char *device_name, bool connected, int wifi_rssi)
{
    ESP_LOGI(TAG, "Showing status screen: %s, connected=%d", device_name, connected);
    clear_screen();

    // Device name
    lv_obj_t *name = lv_label_create(s_current_screen);
    lv_label_set_text(name, device_name);
    lv_obj_set_style_text_color(name, COLOR_TEXT, 0);
    lv_obj_set_style_text_font(name, &lv_font_montserrat_28, 0);
    lv_obj_align(name, LV_ALIGN_CENTER, 0, -40);

    // Status dot + label
    lv_obj_t *status_container = lv_obj_create(s_current_screen);
    lv_obj_set_size(status_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(status_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(status_container, 0, 0);
    lv_obj_set_style_pad_all(status_container, 4, 0);
    lv_obj_set_flex_flow(status_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(status_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(status_container, LV_ALIGN_CENTER, 0, 10);

    lv_obj_t *dot = lv_obj_create(status_container);
    lv_obj_set_size(dot, 10, 10);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dot, connected ? lv_color_hex(0x22c55e) : lv_color_hex(0x6b7280), 0);
    lv_obj_set_style_border_width(dot, 0, 0);

    lv_obj_t *status_label = lv_label_create(status_container);
    lv_label_set_text(status_label, connected ? "Online" : "Offline");
    lv_obj_set_style_text_color(status_label, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_16, 0);

    // WiFi signal
    char wifi_text[32];
    snprintf(wifi_text, sizeof(wifi_text), "WiFi: %d dBm", wifi_rssi);
    lv_obj_t *wifi = lv_label_create(s_current_screen);
    lv_label_set_text(wifi, wifi_text);
    lv_obj_set_style_text_color(wifi, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_font(wifi, &lv_font_montserrat_14, 0);
    lv_obj_align(wifi, LV_ALIGN_CENTER, 0, 50);

    return ESP_OK;
}

esp_err_t display_ui_show_error(const char *message)
{
    ESP_LOGE(TAG, "Showing error: %s", message);
    clear_screen();

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, message);
    lv_obj_set_style_text_color(label, COLOR_ERROR, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(label, 600);
    lv_obj_center(label);

    return ESP_OK;
}
