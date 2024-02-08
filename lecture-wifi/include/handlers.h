#include "bsp.h"


esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err);
esp_err_t root_get_handler(httpd_req_t *req);
esp_err_t counter_get_handler(httpd_req_t *req);
esp_err_t post_handler(httpd_req_t *req);