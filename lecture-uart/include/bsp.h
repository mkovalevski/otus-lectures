#ifndef BSP
#define BSP

#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portable.h"

#define UART_NUM UART_NUM_0
#define TXD_PIN GPIO_NUM_1
#define RXD_PIN GPIO_NUM_3

#endif