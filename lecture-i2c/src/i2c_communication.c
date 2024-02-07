#include "i2c_communication.h"


void i2c_master_init(){
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

void bmp280_read_task(void) {
    uint8_t sensor_data[2];
    while (1) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (BMP280_SENSOR_ADDR << 1) | I2C_MASTER_READ, true);
        i2c_master_read_byte(cmd, &sensor_data[0], I2C_MASTER_ACK);
        i2c_master_read_byte(cmd, &sensor_data[1], I2C_MASTER_NACK);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
        if (ret == ESP_OK) {
            ESP_LOGI("BMP280", "Read Success: %d.%d", sensor_data[0], sensor_data[1]);
        } else {
            ESP_LOGE("BMP280", "Read Fail");
        }
        i2c_cmd_link_delete(cmd);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for a second
    }
}