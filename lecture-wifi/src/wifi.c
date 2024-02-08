#include "wifi.h"



void wifi_event_handler(void * arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_AP_STACONNECTED)
        {
            wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
            ESP_LOGI("SOFT AP", "station %s join, AID=%d", (char *)event->mac, event->aid);
        }
        else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
        {
            wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
            ESP_LOGI("SOFT AP", "station %s leave, AID=%d", (char *)event->mac, event->aid);
        }

        if (event_id == WIFI_EVENT_STA_CONNECTED)
        {
            wifi_event_sta_connected_t *event = (wifi_event_sta_connected_t *)event_data;
            ESP_LOGI("STATION", "SSID of AP: %s", event->ssid);
        }
        else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            wifi_event_sta_connected_t *event = (wifi_event_sta_connected_t *)event_data;
            ESP_LOGI("STATION", "DISCONNECTED FROM %s", event->ssid);
        }
    }
    else if (event_base == IP_EVENT) {
        if (event_id == IP_EVENT_STA_GOT_IP) {
            ip_event_got_ip_t * event = (ip_event_got_ip_t *)event_data;
            ESP_LOGI("STATION", "ESP32 IP: %d.%d.%d.%d", (uint8_t)event->ip_info.ip.addr,
                                                            ((uint8_t)((event->ip_info.ip.addr)>>8 & 0xFF)),
                                                            ((uint8_t)((event->ip_info.ip.addr)>>16 & 0xFF)),
                                                            ((uint8_t)((event->ip_info.ip.addr)>>24 & 0xFF))
                                                            );
        }
    }
}


void wifi_init_sta_ap(void) {
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &wifi_event_handler,
                                        NULL,
                                        NULL);

    esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID,
                                        &wifi_event_handler,
                                        NULL,
                                        NULL);

    wifi_config_t sta_wifi_config = {
        .sta = {
            .ssid = STA_WIFI_SSID,
            .password = STA_WIFI_PASS,
        },
    };
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI("wifi_init_sta", "wifi_init_sta finished.");

    esp_netif_create_default_wifi_ap();

    wifi_config_t ap_wifi_config = {
        .ap = {
            .ssid = AP_WIFI_SSID,
            .ssid_len = strlen(AP_WIFI_SSID),
            .channel = 0,
            .password = AP_WIFI_PASS,
            .max_connection = MAX_CONNS,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    if (strlen(AP_WIFI_PASS) == 0)
    {
        ap_wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}