#include "wifi.h"
#include "uris.h"
#include "tasks.h"

void start_webserver()
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK)
    {
        ESP_LOGI("WEBSERVER", "Registering URI handlers");
        httpd_register_uri_handler(server, &root);
        httpd_register_uri_handler(server, &counter_uri);
        httpd_register_uri_handler(server, &post_uri);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }
}

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_sta_ap();
    start_webserver();

    xTaskCreate(counter_task, "counter", 2048, NULL, 5, NULL);
}