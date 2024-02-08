#include "uris.h"


httpd_uri_t root = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = root_get_handler,
    .user_ctx  = NULL
};

httpd_uri_t counter_uri = {
    .uri       = "/counter",
    .method    = HTTP_GET,
    .handler   = counter_get_handler,
    .user_ctx  = NULL
};

httpd_uri_t post_uri = {
    .uri      = "/post",
    .method   = HTTP_POST,
    .handler  = post_handler,
    .user_ctx = NULL
};