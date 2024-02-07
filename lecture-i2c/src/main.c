#include "i2c_communication.h"


void app_main() {
    i2c_master_init();
    xTaskCreate(bmp280_read_task, "bmp280_read_task", 2048, NULL, 10, NULL);
}