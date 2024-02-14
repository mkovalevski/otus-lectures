#include "uart_test.h"
#include "spi.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "string.h"


#define INPUT_PIN GPIO_NUM_13

static QueueHandle_t gpio_evt_queue = NULL;

void task_mpu6050(void *ignore);

void IRAM_ATTR gpio_interrupt_handler(void * args) {
    // static DRAM_ATTR int counter = 0;

    // while(counter < 0x4C4B400) {
    //     counter++;
    //     ESP_DRAM_LOGE("INTERRUPT HANDLER", "%d", counter);
    // }


    int pin_number = (int)args;
    xQueueSendFromISR(gpio_evt_queue, &pin_number, NULL);
}

void gpio_task(void * params) {
    int pin_number = 0;

    while (1) {
        if (xQueueReceive(gpio_evt_queue, &pin_number, portMAX_DELAY)) {
                int counter = 0;
                while(counter < 256) {
                    counter++;
                    ESP_LOGI("INT TASK", "%d", counter);
                }
        }
    }
}



void uart_init() {
    const uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .source_clk = UART_SCLK_DEFAULT,
        .rx_flow_ctrl_thresh = UART_HW_FLOWCTRL_DISABLE
    };

    uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, -1, -1);
}

void uart_test() {
    uint8_t * tmp_str = malloc(2048);
    memset(tmp_str, 'O', 2048);
    
    uint32_t start_time = esp_timer_get_time();
    uart_write_bytes(UART_NUM, tmp_str, 2048);
    uint32_t end_time = esp_timer_get_time();
    ESP_LOGI("UART TX DELAY", "Время выполнения: %lu мкс\n", end_time - start_time);

    uint8_t tmp_buf[64];

    start_time = esp_timer_get_time();
    int rx_bytes = uart_read_bytes(UART_NUM, tmp_buf, sizeof(tmp_buf), pdMS_TO_TICKS(1000));
    end_time = esp_timer_get_time();
    ESP_LOGI("UART RX DELAY", "Время выполнения: %lu мкс\n", end_time - start_time);
    ESP_LOGI("UART_RX", "Read %d bytes: '%s'", rx_bytes, tmp_buf);

    free(tmp_str);
}

void config_button() {
    gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(INPUT_PIN);
    gpio_pulldown_dis(INPUT_PIN);
    gpio_set_intr_type(INPUT_PIN, GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(INPUT_PIN, gpio_interrupt_handler, (void *)INPUT_PIN);
}


void spi_bus_init() {
    spi_bus_config_t spi_bus_config = {
        .miso_io_num = PIN_NUM_MISO,
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
        .clock_speed_hz = 1*1000*1000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1
    };

    spi_bus_add_device(HSPI_HOST, &dev_config, hSpi);
}

void spi_send_receive(void * params, uint8_t str_num, uint8_t dots_num) {
    spi_device_handle_t spi_params = (spi_device_handle_t)params;


    uint8_t data_to_send[2] = {str_num, dots_num};
    uint8_t data_rx[64];
    spi_transaction_t transaction;

    memset(&transaction, 0, sizeof(transaction));
    transaction.length = 8 * sizeof(data_to_send);
    transaction.tx_buffer = data_to_send;
    transaction.rx_buffer = data_rx;             

    uint32_t start_time = esp_timer_get_time();
    ESP_ERROR_CHECK(spi_device_transmit(spi_params, &transaction));
    uint32_t end_time = esp_timer_get_time();
    ESP_LOGI("SPI DELAY", "Время выполнения: %lu микросекунд\n", end_time - start_time);

    ESP_LOGI("SPI", "Sent data:");
    for (int i = 0; i < sizeof(data_to_send); i++) {
        ESP_LOGI("SPI", "%02X ", data_to_send[i]);
    }
}


void app_main() {
    spi_device_handle_t spi_handle;

    uart_init();
    spi_bus_init();
    spi_bus_config_device(&spi_handle);


    config_button();

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 1, NULL);
    xTaskCreate(task_mpu6050, "task_mpu6050", 2048, NULL, 5, NULL);
    
    
    while(1) {
        uart_test();

        for (uint8_t i=1; i<8; i++) {
            uint8_t j = 0;
            uint8_t dot = 0;
            while (j<0x8) {
                dot = 1<<j;
                spi_send_receive((void *)spi_handle, i, dot);
                vTaskDelay(pdMS_TO_TICKS(1000));
                dot = 0;
                j++;
            }
        }


        vTaskDelay(pdMS_TO_TICKS(1000));
    }


}