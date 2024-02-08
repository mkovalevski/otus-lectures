#include "tasks.h"


int counter = 0;

void counter_task(void *pvParameter) {
    while (1) {
        counter++;
        ESP_LOGI("RTOS TASK", "Counter: %d", counter);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Ждем 1 секунду
    }
}