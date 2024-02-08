#ifndef BSP
#define BSP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_server.h"



#define AP_WIFI_SSID      "ESP32-Access-Point"
#define AP_WIFI_PASS      "123456789"
#define MAX_CONNS       4

#define STA_WIFI_SSID      "RT-WiFi-909C"
#define STA_WIFI_PASS      "zy7c7cTu"

#endif