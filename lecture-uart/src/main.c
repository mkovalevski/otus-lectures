#include "uart_communication.h"


void app_main() {
    UartWriteTask_Args * uart_args = pvPortMalloc(sizeof(UartWriteTask_Args));

    uart_args->log_name = "UART_TX";
    uart_args->data = "Hello FREERTOS!";

    uart_initialize();

    xTaskCreate(uart_read_data, "uart_read_data", 4096, NULL, 5, NULL);
    xTaskCreate(uart_write_data, "uart_write_data", 1024, (void *)uart_args, 5, NULL);
}