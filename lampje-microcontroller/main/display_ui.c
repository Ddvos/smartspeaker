#include "display_ui.h"
#include "display_driver.h"
#include "audio_codecs.h"
#include "lvgl.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "display_ui";
static display_ui_music_cb_t s_music_cb = NULL;

void display_ui_set_music_callback(display_ui_music_cb_t cb) {
    s_music_cb = cb;
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
}

static lv_obj_t *s_music_btn_label = NULL;

static void music_btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_CLICKED) return;

    if (audio_codecs_is_playing_music()) {
        audio_codecs_stop_music();
        if (s_music_btn_label) lv_label_set_text(s_music_btn_label, LV_SYMBOL_PLAY " Test");
    } else {
        audio_codecs_play_music();
        if (s_music_btn_label) lv_label_set_text(s_music_btn_label, LV_SYMBOL_PAUSE " Stop");
    }
}

esp_err_t display_ui_show_voice_idle(void)
{
    clear_screen();

    create_voice_orb(s_current_screen, COLOR_PRIMARY, 200);

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, "Tik om te praten");
    lv_obj_set_style_text_color(label, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 140);

    // Play/pause test music button
    lv_obj_t *btn = lv_btn_create(s_current_screen);
    lv_obj_set_size(btn, 160, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 210);
    lv_obj_set_style_bg_color(btn, COLOR_SURFACE, 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(btn, 25, 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_border_color(btn, COLOR_TEXT_DIM, 0);
    lv_obj_add_event_cb(btn, music_btn_event_cb, LV_EVENT_CLICKED, NULL);

    s_music_btn_label = lv_label_create(btn);
    lv_label_set_text(s_music_btn_label,
                      audio_codecs_is_playing_music() ? LV_SYMBOL_PAUSE " Stop" : LV_SYMBOL_PLAY " Test");
    lv_obj_set_style_text_color(s_music_btn_label, COLOR_TEXT, 0);
    lv_obj_set_style_text_font(s_music_btn_label, &lv_font_montserrat_16, 0);
    lv_obj_center(s_music_btn_label);

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
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 140);

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

    lv_obj_t *label = lv_label_create(s_current_screen);
    lv_label_set_text(label, "Denkt na...");
    lv_obj_set_style_text_color(label, COLOR_THINKING, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 140);

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
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 140);

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
