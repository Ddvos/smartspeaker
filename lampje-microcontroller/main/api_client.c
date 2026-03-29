#include "api_client.h"
#include "esp_http_client.h"
#include "esp_tls.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "api_client";

#define MAX_RESPONSE_LEN 2048

typedef struct {
    char *buffer;
    int len;
    int max_len;
} response_buffer_t;

static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    response_buffer_t *resp = (response_buffer_t *)evt->user_data;

    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            if (resp && resp->buffer && (resp->len + evt->data_len < resp->max_len)) {
                memcpy(resp->buffer + resp->len, evt->data, evt->data_len);
                resp->len += evt->data_len;
                resp->buffer[resp->len] = '\0';
            }
            break;
        default:
            break;
    }
    return ESP_OK;
}

esp_err_t api_register_device(const char *chip_id, const char *firmware_version,
                               register_response_t *response)
{
    char url[256];
    snprintf(url, sizeof(url), "%s/api/devices/register", API_BASE_URL);

    cJSON *body = cJSON_CreateObject();
    cJSON_AddStringToObject(body, "chipId", chip_id);
    cJSON_AddStringToObject(body, "firmwareVersion", firmware_version);
    char *post_data = cJSON_PrintUnformatted(body);
    cJSON_Delete(body);

    char *resp_buf = calloc(1, MAX_RESPONSE_LEN);
    response_buffer_t resp = { .buffer = resp_buf, .len = 0, .max_len = MAX_RESPONSE_LEN };

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .event_handler = http_event_handler,
        .user_data = &resp,
        .skip_cert_common_name_check = true,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);

    if (err == ESP_OK && status == 200) {
        cJSON *json = cJSON_Parse(resp_buf);
        if (json) {
            cJSON *id = cJSON_GetObjectItem(json, "deviceId");
            cJSON *token = cJSON_GetObjectItem(json, "deviceToken");
            cJSON *code = cJSON_GetObjectItem(json, "pairingCode");
            cJSON *expires = cJSON_GetObjectItem(json, "pairingExpiresAt");

            if (id) strncpy(response->device_id, id->valuestring, sizeof(response->device_id) - 1);
            if (token) strncpy(response->device_token, token->valuestring, sizeof(response->device_token) - 1);
            if (code) strncpy(response->pairing_code, code->valuestring, sizeof(response->pairing_code) - 1);
            if (expires) strncpy(response->pairing_expires_at, expires->valuestring, sizeof(response->pairing_expires_at) - 1);

            cJSON_Delete(json);
            ESP_LOGI(TAG, "Registered device %s, pairing code: %s", response->device_id, response->pairing_code);
        }
    } else {
        ESP_LOGE(TAG, "Register failed: HTTP %d, err=%s", status, esp_err_to_name(err));
        err = ESP_FAIL;
    }

    esp_http_client_cleanup(client);
    free(post_data);
    free(resp_buf);
    return err;
}

esp_err_t api_send_heartbeat(const char *device_id, const char *token,
                              int wifi_rssi, const char *ip_address,
                              const char *firmware_version,
                              heartbeat_response_t *response)
{
    char url[256];
    snprintf(url, sizeof(url), "%s/api/devices/%s/heartbeat", API_BASE_URL, device_id);

    char auth_header[192];
    snprintf(auth_header, sizeof(auth_header), "Bearer %s", token);

    cJSON *body = cJSON_CreateObject();
    cJSON_AddNumberToObject(body, "wifiRssi", wifi_rssi);
    cJSON_AddStringToObject(body, "ipAddress", ip_address);
    cJSON_AddStringToObject(body, "firmwareVersion", firmware_version);
    char *post_data = cJSON_PrintUnformatted(body);
    cJSON_Delete(body);

    char *resp_buf = calloc(1, MAX_RESPONSE_LEN);
    response_buffer_t resp = { .buffer = resp_buf, .len = 0, .max_len = MAX_RESPONSE_LEN };

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .event_handler = http_event_handler,
        .user_data = &resp,
        .skip_cert_common_name_check = true,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Authorization", auth_header);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    response->http_status = status;

    if (err == ESP_OK && status == 200) {
        cJSON *json = cJSON_Parse(resp_buf);
        if (json) {
            cJSON *paired = cJSON_GetObjectItem(json, "paired");
            if (paired) {
                response->paired = cJSON_IsTrue(paired);
            }
            cJSON *updated = cJSON_GetObjectItem(json, "configUpdatedAt");
            if (updated && updated->valuestring) {
                strncpy(response->config_updated_at, updated->valuestring,
                        sizeof(response->config_updated_at) - 1);
            }
            cJSON_Delete(json);
        }
    } else {
        ESP_LOGE(TAG, "Heartbeat failed: HTTP %d", status);
        err = ESP_FAIL;
    }

    esp_http_client_cleanup(client);
    free(post_data);
    free(resp_buf);
    return err;
}

esp_err_t api_get_config(const char *device_id, const char *token,
                          device_config_t *config)
{
    char url[256];
    snprintf(url, sizeof(url), "%s/api/devices/%s", API_BASE_URL, device_id);

    char auth_header[192];
    snprintf(auth_header, sizeof(auth_header), "Bearer %s", token);

    char *resp_buf = calloc(1, MAX_RESPONSE_LEN);
    response_buffer_t resp = { .buffer = resp_buf, .len = 0, .max_len = MAX_RESPONSE_LEN };

    esp_http_client_config_t config_http = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .event_handler = http_event_handler,
        .user_data = &resp,
        .skip_cert_common_name_check = true,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config_http);
    esp_http_client_set_header(client, "Authorization", auth_header);

    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);

    if (err == ESP_OK && status == 200) {
        cJSON *json = cJSON_Parse(resp_buf);
        if (json) {
            cJSON *cfg = cJSON_GetObjectItem(json, "config");
            if (cfg) {
                cJSON *leds = cJSON_GetObjectItem(cfg, "ledColors");
                if (leds) {
                    cJSON *c;
                    c = cJSON_GetObjectItem(leds, "idle");
                    if (c) strncpy(config->led_idle, c->valuestring, sizeof(config->led_idle) - 1);
                    c = cJSON_GetObjectItem(leds, "listening");
                    if (c) strncpy(config->led_listening, c->valuestring, sizeof(config->led_listening) - 1);
                    c = cJSON_GetObjectItem(leds, "thinking");
                    if (c) strncpy(config->led_thinking, c->valuestring, sizeof(config->led_thinking) - 1);
                    c = cJSON_GetObjectItem(leds, "speaking");
                    if (c) strncpy(config->led_speaking, c->valuestring, sizeof(config->led_speaking) - 1);
                    c = cJSON_GetObjectItem(leds, "error");
                    if (c) strncpy(config->led_error, c->valuestring, sizeof(config->led_error) - 1);
                }
                cJSON *mic = cJSON_GetObjectItem(cfg, "micSensitivity");
                if (mic) config->mic_sensitivity = mic->valueint;
                cJSON *vol = cJSON_GetObjectItem(cfg, "speakerVolume");
                if (vol) config->speaker_volume = vol->valueint;
                cJSON *bright = cJSON_GetObjectItem(cfg, "displayBrightness");
                if (bright) config->display_brightness = bright->valueint;
            }
            cJSON_Delete(json);
            ESP_LOGI(TAG, "Config fetched successfully");
        }
    } else {
        ESP_LOGE(TAG, "Get config failed: HTTP %d", status);
        err = ESP_FAIL;
    }

    esp_http_client_cleanup(client);
    free(resp_buf);
    return err;
}

esp_err_t api_get_gemini_token(const char *device_id, const char *token,
                                gemini_token_response_t *response)
{
    char url[256];
    snprintf(url, sizeof(url), "%s/api/devices/%s/gemini-token", API_BASE_URL, device_id);

    char auth_header[192];
    snprintf(auth_header, sizeof(auth_header), "Bearer %s", token);

    char *resp_buf = calloc(1, MAX_RESPONSE_LEN);
    response_buffer_t resp = { .buffer = resp_buf, .len = 0, .max_len = MAX_RESPONSE_LEN };

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .event_handler = http_event_handler,
        .user_data = &resp,
        .skip_cert_common_name_check = true,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Authorization", auth_header);
    esp_http_client_set_post_field(client, "{}", 2);

    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);

    if (err == ESP_OK && status == 200) {
        cJSON *json = cJSON_Parse(resp_buf);
        if (json) {
            cJSON *t = cJSON_GetObjectItem(json, "token");
            if (t && t->valuestring)
                strncpy(response->gemini_token, t->valuestring, sizeof(response->gemini_token) - 1);
            cJSON *m = cJSON_GetObjectItem(json, "model");
            if (m && m->valuestring)
                strncpy(response->model, m->valuestring, sizeof(response->model) - 1);
            cJSON *v = cJSON_GetObjectItem(json, "voice");
            if (v && v->valuestring)
                strncpy(response->voice, v->valuestring, sizeof(response->voice) - 1);
            cJSON *sp = cJSON_GetObjectItem(json, "systemPrompt");
            if (sp && sp->valuestring)
                strncpy(response->system_prompt, sp->valuestring, sizeof(response->system_prompt) - 1);
            cJSON *ea = cJSON_GetObjectItem(json, "expiresAt");
            if (ea && ea->valuestring)
                strncpy(response->expires_at, ea->valuestring, sizeof(response->expires_at) - 1);
            cJSON *sv = cJSON_GetObjectItem(json, "speakerVolume");
            if (sv) response->speaker_volume = sv->valueint;
            cJSON *ms = cJSON_GetObjectItem(json, "micSensitivity");
            if (ms) response->mic_sensitivity = ms->valueint;
            cJSON_Delete(json);
            ESP_LOGI(TAG, "Gemini token fetched, model: %s", response->model);
        }
    } else {
        ESP_LOGE(TAG, "Get Gemini token failed: HTTP %d", status);
        err = ESP_FAIL;
    }

    esp_http_client_cleanup(client);
    free(resp_buf);
    return err;
}

esp_err_t api_refresh_pairing_code(const char *device_id, const char *token,
                                    pairing_code_response_t *response)
{
    char url[256];
    snprintf(url, sizeof(url), "%s/api/devices/%s/pairing-code/refresh", API_BASE_URL, device_id);

    char auth_header[192];
    snprintf(auth_header, sizeof(auth_header), "Bearer %s", token);

    char *resp_buf = calloc(1, MAX_RESPONSE_LEN);
    response_buffer_t resp = { .buffer = resp_buf, .len = 0, .max_len = MAX_RESPONSE_LEN };

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .event_handler = http_event_handler,
        .user_data = &resp,
        .skip_cert_common_name_check = true,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Authorization", auth_header);
    esp_http_client_set_post_field(client, "{}", 2);

    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);

    if (err == ESP_OK && status == 200) {
        cJSON *json = cJSON_Parse(resp_buf);
        if (json) {
            cJSON *code = cJSON_GetObjectItem(json, "code");
            cJSON *expires = cJSON_GetObjectItem(json, "expiresAt");
            if (code) strncpy(response->code, code->valuestring, sizeof(response->code) - 1);
            if (expires) strncpy(response->expires_at, expires->valuestring, sizeof(response->expires_at) - 1);
            cJSON_Delete(json);
        }
    } else {
        ESP_LOGE(TAG, "Refresh pairing code failed: HTTP %d", status);
        err = ESP_FAIL;
    }

    esp_http_client_cleanup(client);
    free(resp_buf);
    return err;
}
