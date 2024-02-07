#ifndef BSP
#define BSP

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_MASTER_SCL_IO    GPIO_NUM_22
#define I2C_MASTER_SDA_IO    GPIO_NUM_21
#define I2C_MASTER_NUM       I2C_NUM_0
#define I2C_MASTER_FREQ_HZ   100000
#define I2C_MASTER_TIMEOUT_MS       1000

#define BMP280_SENSOR_ADDR   0x76

#endif