// i2c delay main

// The sensor used in this example is an MSA311.

// This code is in the Public Domain (or CC0 licensed, at your option.)

#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "i2c-delay";

#define I2C_MASTER_SCL_IO           34      // Adafruit Metro ESP32-S2 SCL pin
#define I2C_MASTER_SDA_IO           33      // Adafruit Metro ESP32-S2 SDA pin

#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */

#define I2C_MASTER_FREQ_HZ          100000                     /*!< I2C master clock frequency */
//#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */

#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define SENSOR_ADDR                 0x62   // I2C address of the MSA311 sensor


#define REGISTER_TO_WRITE           0x0F   // MSA311 resolution register. This is just some random place to write.

static esp_err_t i2c_write(uint8_t *data, size_t len)
{
    esp_err_t ret;

    ret = i2c_master_write_to_device(I2C_MASTER_NUM, SENSOR_ADDR, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);

    return ret;
}

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}


void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");


    while (1) {
        uint8_t write_buf[4] = {REGISTER_TO_WRITE, 0, 0, 0};
        esp_err_t ret = i2c_write(write_buf, sizeof(write_buf));
        if (ret) {
            ESP_LOGE(TAG, "i2c_write() status: %s", esp_err_to_name(ret));
        }
    }


}
