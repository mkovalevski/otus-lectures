#include "uart_communication.h"
#include "string.h"


void uart_initialize() {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .source_clk = UART_SCLK_DEFAULT,
        .rx_flow_ctrl_thresh = UART_HW_FLOWCTRL_DISABLE
    };

    uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}


void uart_write_data(void * pvParameters) {
    UartWriteTask_Args * args = (UartWriteTask_Args *)pvParameters;

    const int len = strlen(args->data);

    while(1) {
        const int tx_bytes = uart_write_bytes(UART_NUM_0, args->data, len);
        ESP_LOGI(args->log_name, "Wrote %d bytes", tx_bytes);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void uart_read_data() {
    uint8_t * data = (uint8_t *)malloc(1024);

    while(1) {
        const int rx_bytes = uart_read_bytes(UART_NUM, data, 1024, 1000 / portTICK_PERIOD_MS);
        if (rx_bytes > 0) {
            data[rx_bytes] = 0;
            ESP_LOGI("UART_RX", "Read %d bytes: %s", rx_bytes, data);
        }
    }

    free(data);
}