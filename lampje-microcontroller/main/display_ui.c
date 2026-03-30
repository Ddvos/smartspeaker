#include "display_ui.h"
#include "display_driver.h"
#include "lvgl.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "display_ui";
static display_ui_voice_cb_t s_start_cb = NULL;
static display_ui_voice_cb_t s_stop_cb = NULL;

void display_ui_set_voice_callbacks(display_ui_voice_cb_t start_cb, display_ui_voice_cb_t stop_cb) {
    s_start_cb = start_cb;
    s_stop_cb = stop_cb;
}

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

// ── Voice interaction screens ─────────────────────────────────

#define COLOR_LISTENING  lv_color_hex(0x00d4ec)
#define COLOR_THINKING   lv_color_hex(0xa68cff)
#define COLOR_SPEAKING   lv_color_hex(0xff7520)

static void create_voice_orb(lv_obj_t *parent, lv_color_t color, int size)
{
    lv_obj_t *orb = lv_obj_create(parent);
    lv_obj_set_size(orb, size, size);
    lv_obj_set_style_radius(orb, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(orb, color, 0);
    lv_obj_set_style_bg_opa(orb, LV_OPA_30, 0);
    lv_obj_set_style_border_width(orb, 2, 0);
    lv_obj_set_style_border_color(orb, color, 0);
    lv_obj_set_style_border_opa(orb, LV_OPA_60, 0);
    lv_obj_center(orb);
    lv_obj_remove_flag(orb, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
}

static void start_btn_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;
    ESP_LOGI(TAG, "Start button pressed");
    if (s_start_cb) s_start_cb();
}

static void stop_btn_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;
    ESP_LOGI(TAG, "Stop button pressed");
    if (s_stop_cb) s_stop_cb();
}

static void create_voice_buttons(lv_obj_t *parent, bool active)
{
    // Start button (left)
    lv_obj_t *start_btn = lv_btn_create(parent);
    lv_obj_set_size(start_btn, 130, 50);
    lv_obj_align(start_btn, LV_ALIGN_CENTER, -75, 180);
    lv_obj_set_style_radius(start_btn, 25, 0);
    lv_obj_set_style_border_width(start_btn, 1, 0);
    if (!active) {
        // Idle: start is highlighted
        lv_obj_set_style_bg_color(start_btn, COLOR_PRIMARY, 0);
        lv_obj_set_style_bg_opa(start_btn, LV_OPA_COVER, 0);
        lv_obj_set_style_border_color(start_btn, COLOR_PRIMARY, 0);
    } else {
        // Active: start is dimmed
        lv_obj_set_style_bg_color(start_btn, COLOR_SURFACE, 0);
        lv_obj_set_style_bg_opa(start_btn, LV_OPA_COVER, 0);
        lv_obj_set_style_border_color(start_btn, COLOR_TEXT_DIM, 0);
    }
    lv_obj_add_event_cb(start_btn, start_btn_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, LV_SYMBOL_PLAY " Start");
    lv_obj_set_style_text_color(start_label, !active ? COLOR_BG : COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_font(start_label, &lv_font_montserrat_16, 0);
    lv_obj_center(start_label);

    // Stop button (right)
    lv_obj_t *stop_btn = lv_btn_create(parent);
    lv_obj_set_size(stop_btn, 130, 50);
    lv_obj_align(stop_btn, LV_ALIGN_CENTER, 75, 180);
    lv_obj_set_style_radius(stop_btn, 25, 0);
    lv_obj_set_style_border_width(stop_btn, 1, 0);
    if (active) {
        // Active: stop is highlighted
        lv_obj_set_style_bg_color(stop_btn, COLOR_ERROR, 0);
        lv_obj_set_style_bg_opa(stop_btn, LV_OPA_COVER, 0);
        lv_obj_set_style_border_color(stop_btn, COLOR_ERROR, 0);
    } else {
        // Idle: stop is dimmed
        lv_obj_set_style_bg_color(stop_btn, COLOR_SURFACE, 0);
        lv_obj_set_style_bg_opa(stop_btn, LV_OPA_COVER, 0);
        lv_obj_set_style_border_color(stop_btn, COLOR_TEXT_DIM, 0);
    }
    lv_obj_add_event_cb(stop_btn, stop_btn_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *stop_label = lv_label_create(stop_btn);
    lv_label_set_text(stop_label, LV_SYMBOL_STOP " Stop");
    lv_obj_set_style_text_color(stop_label, active ? COLOR_TEXT : COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_font(stop_label, &lv_font_montserrat_16, 0);
    lv_obj_center(stop_label);
}

esp_err_t display_ui_show_voice_idle(void)
{
    clear_screen();

    create_voice_orb(s_current_screen, COLOR_PRIMARY, 200);

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, "Klaar om te praten");
    lv_obj_set_style_text_color(label, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 130);

    create_voice_buttons(s_current_screen, false);

    return ESP_OK;
}

esp_err_t display_ui_show_voice_listening(void)
{
    clear_screen();

    create_voice_orb(s_current_screen, COLOR_LISTENING, 240);

    // Pulsing inner orb
    lv_obj_t *inner = lv_obj_create(s_current_screen);
    lv_obj_set_size(inner, 120, 120);
    lv_obj_set_style_radius(inner, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(inner, COLOR_LISTENING, 0);
    lv_obj_set_style_bg_opa(inner, LV_OPA_50, 0);
    lv_obj_set_style_border_width(inner, 0, 0);
    lv_obj_center(inner);
    lv_obj_remove_flag(inner, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    // Pulse animation
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, inner);
    lv_anim_set_values(&a, 120, 160);
    lv_anim_set_duration(&a, 800);
    lv_anim_set_playback_duration(&a, 800);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_width);
    lv_anim_start(&a);

    lv_anim_t a2 = a;
    lv_anim_set_exec_cb(&a2, (lv_anim_exec_xcb_t)lv_obj_set_height);
    lv_anim_start(&a2);

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, "Luistert...");
    lv_obj_set_style_text_color(label, COLOR_LISTENING, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 130);

    create_voice_buttons(s_current_screen, true);

    return ESP_OK;
}

esp_err_t display_ui_show_voice_thinking(void)
{
    clear_screen();

    create_voice_orb(s_current_screen, COLOR_THINKING, 200);

    lv_obj_t *spinner = lv_spinner_create(s_current_screen);
    lv_obj_set_size(spinner, 80, 80);
    lv_obj_center(spinner);
    lv_spinner_set_anim_params(spinner, 1200, 200);
    lv_obj_remove_flag(spinner, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, "Denkt na...");
    lv_obj_set_style_text_color(label, COLOR_THINKING, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 130);

    create_voice_buttons(s_current_screen, true);

    return ESP_OK;
}

esp_err_t display_ui_show_voice_speaking(void)
{
    clear_screen();

    create_voice_orb(s_current_screen, COLOR_SPEAKING, 240);

    // Animated inner orb
    lv_obj_t *inner = lv_obj_create(s_current_screen);
    lv_obj_set_size(inner, 140, 140);
    lv_obj_set_style_radius(inner, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(inner, COLOR_SPEAKING, 0);
    lv_obj_set_style_bg_opa(inner, LV_OPA_40, 0);
    lv_obj_set_style_border_width(inner, 0, 0);
    lv_obj_center(inner);
    lv_obj_remove_flag(inner, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, inner);
    lv_anim_set_values(&a, 140, 180);
    lv_anim_set_duration(&a, 600);
    lv_anim_set_playback_duration(&a, 600);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_width);
    lv_anim_start(&a);

    lv_anim_t a2 = a;
    lv_anim_set_exec_cb(&a2, (lv_anim_exec_xcb_t)lv_obj_set_height);
    lv_anim_start(&a2);

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, "Spreekt...");
    lv_obj_set_style_text_color(label, COLOR_SPEAKING, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 130);

    create_voice_buttons(s_current_screen, true);

    return ESP_OK;
}

esp_err_t display_ui_show_voice_connecting(void)
{
    clear_screen();

    lv_obj_t *spinner = lv_spinner_create(s_current_screen);
    lv_obj_set_size(spinner, 60, 60);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, -30);
    lv_spinner_set_anim_params(spinner, 1000, 200);

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, "Verbinden met AI...");
    lv_obj_set_style_text_color(label, COLOR_TEXT, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 30);

    return ESP_OK;
}
