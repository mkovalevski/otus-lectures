#ifndef UART_COM
#define UART_COM

#include "bsp.h"
#include "esp_log.h"

typedef struct {
    char * log_name;
    char * data;
} UartWriteTask_Args;

void uart_initialize(void);
void uart_read_data(void);
void uart_write_data(void * pvParameters);

#endif