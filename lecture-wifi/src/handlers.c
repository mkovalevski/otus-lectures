#include "handlers.h"
#include "tasks.h"


esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (err == HTTPD_404_NOT_FOUND) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Not found");
    }
    return ESP_FAIL;
}


esp_err_t root_get_handler(httpd_req_t *req) {
    httpd_resp_send(req, "<h1>Hello from ESP32!</h1>", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t counter_get_handler(httpd_req_t *req) {
    char resp[64];
    snprintf(resp, sizeof(resp), "Counter value: %d", counter);
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret, remaining = req->content_len;

    /* Read data received in the POST request */
    while (remaining > 0) {
        if ((ret = httpd_req_recv(req, buf, remaining)) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL;
        }
        remaining -= ret;
        /* Process data as needed */
        // Example: Print received data
        ESP_LOGI("POST", "%.*s", ret, buf);
    }

    /* Send response back to client */
    const char *resp_str = "Data received successfully";
    httpd_resp_send(req, resp_str, strlen(resp_str));

    return ESP_OK;
}