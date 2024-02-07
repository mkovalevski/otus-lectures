#include "spi_communication.h"
#include "string.h"
#include "esp_log.h"

void spi_bus_init() {
    spi_bus_config_t spi_bus_config = {
        .miso_io_num = -1,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadhd_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0
    };

    spi_bus_initialize(HSPI_HOST, &spi_bus_config, SPI_DMA_CH_AUTO);
}


void spi_bus_config_device(spi_device_handle_t * hSpi) {
    spi_device_interface_config_t dev_config = {
        .clock_speed_hz = 10*1000*1000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1
    };

    spi_bus_add_device(HSPI_HOST, &dev_config, hSpi);
}


void matrix_display_task(void * params) {
    spi_device_handle_t spi_params = (spi_device_handle_t)params;

    uint8_t data1[2] = {0x03, 0xFF};
    uint8_t data2[2] = {0x02, 0x36};


    spi_transaction_t transaction;
    memset(&transaction, 0, sizeof(transaction));
    transaction.length = 16;
    transaction.tx_buffer = data1;
    spi_device_transmit(spi_params, &transaction);
    transaction.tx_buffer = data2;
    spi_device_transmit(spi_params, &transaction);


    while(1) {
        transaction.tx_buffer = data1;
        spi_device_transmit(spi_params, &transaction);
        ESP_LOGI("SPI", "Wrote data1");
        vTaskDelay(pdMS_TO_TICKS(1000));
        transaction.tx_buffer = data2;
        spi_device_transmit(spi_params, &transaction);
        ESP_LOGI("SPI", "Wrote data2");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
