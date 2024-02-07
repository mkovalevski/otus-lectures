#include "spi_communication.h"


void app_main() {
    spi_device_handle_t spi_handle;
    spi_bus_init();
    spi_bus_config_device(&spi_handle);


    xTaskCreate(matrix_display_task, "matrix_display_task", 2048, (void*)spi_handle, 5, NULL);
}