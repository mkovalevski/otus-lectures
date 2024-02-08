#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_spp_api.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define SPP_TAG "SPP"
#define SPP_SERVER_NAME "SPP_SERVER"

void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
//     switch (event)
//     {
//     case ESP_BT_GAP_AUTH_CMPL_EVT:
//     {
//         if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS)
//         {
//             ESP_LOGI(SPP_TAG, "authentication success: %s", param->auth_cmpl.device_name);
//             esp_log_buffer_hex(SPP_TAG, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
//         }
//         else
//         {
//             ESP_LOGE(SPP_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
//         }
//         break;
//     }
//     case ESP_BT_GAP_PIN_REQ_EVT:
//     {
//         ESP_LOGI(SPP_TAG, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
//         if (param->pin_req.min_16_digit)
//         {
//             ESP_LOGI(SPP_TAG, "Input pin code: 0000 0000 0000 0000");
//             esp_bt_pin_code_t pin_code = {0};
//             esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
//         }
//         else
//         {
//             ESP_LOGI(SPP_TAG, "Input pin code: 1234");
//             esp_bt_pin_code_t pin_code;
//             pin_code[0] = '1';
//             pin_code[1] = '2';
//             pin_code[2] = '3';
//             pin_code[3] = '4';
//             esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
//         }
//         break;
//     }

// #if (CONFIG_BT_SSP_ENABLED == true)
//     case ESP_BT_GAP_CFM_REQ_EVT:
//         ESP_LOGI(SPP_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %d", (int)param->cfm_req.num_val);
//         esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
//         break;
//     case ESP_BT_GAP_KEY_NOTIF_EVT:
//         ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%d", (int)param->key_notif.passkey);
//         break;
//     case ESP_BT_GAP_KEY_REQ_EVT:
//         ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
//         break;
// #endif

//     default:
//     {
//         ESP_LOGI(SPP_TAG, "event: %d", event);
//         break;
//     }
//     }
//     return;
}

void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch (event)
    {
    case ESP_SPP_INIT_EVT:
        ESP_LOGI("SPP_CB", "ESP_SPP_INIT_EVT");
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
        esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, SPP_SERVER_NAME);
        break;
    case ESP_SPP_DATA_IND_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_DATA_IND_EVT len=%d handle=%d",
                 (int)param->data_ind.len, (int)param->data_ind.handle);
        esp_log_buffer_hex("", param->data_ind.data, param->data_ind.len);
        break;
    default:
        ESP_LOGI("SPP_CB", "DEFAULT");
        break;
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));

    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_ERROR_CHECK(esp_spp_register_callback(esp_spp_cb));
    ESP_ERROR_CHECK(esp_spp_init(ESP_SPP_MODE_CB));

    ESP_ERROR_CHECK(esp_bt_gap_register_callback(esp_bt_gap_cb));
}
