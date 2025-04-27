#include "ds3231.h"
#include <string.h>
#include "esp_log.h"


#define I2CDEV_TIMEOUT 1000

DS3231::DS3231(i2c_master_bus_handle_t busHandle, uint8_t address, uint32_t clk_speed)
    : i2c_busHandle(busHandle), dev_addr(address), clk_speed(clk_speed)
{
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = clk_speed,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_busHandle, &dev_cfg, &i2c_devHandle));
}

uint8_t DS3231::bcd2dec(uint8_t val) const {
    return (val >> 4) * 10 + (val & 0x0f);
}

uint8_t DS3231::dec2bcd(uint8_t val) const {
    return ((val / 10) << 4) + (val % 10);
}

esp_err_t DS3231::setTime(const struct tm& time) {
    uint8_t data[7];
    data[0] = dec2bcd(time.tm_sec);
    data[1] = dec2bcd(time.tm_min);
    data[2] = dec2bcd(time.tm_hour);
    data[3] = dec2bcd(time.tm_wday + 1);
    data[4] = dec2bcd(time.tm_mday);
    data[5] = dec2bcd(time.tm_mon + 1);
    data[6] = dec2bcd(time.tm_year - 2000);

    return i2c_master_transmit(i2c_devHandle, 
                               (const uint8_t[]){DS3231_ADDR_TIME}, 1, 
                               I2CDEV_TIMEOUT / portTICK_PERIOD_MS) == ESP_OK &&
           i2c_master_transmit(i2c_devHandle, data, 7, I2CDEV_TIMEOUT / portTICK_PERIOD_MS);
}

esp_err_t DS3231::getTime(struct tm& time) {
    uint8_t reg = DS3231_ADDR_TIME;
    uint8_t data[7];

    esp_err_t res = i2c_master_transmit_receive(i2c_devHandle, &reg, 1, data, 7, I2CDEV_TIMEOUT / portTICK_PERIOD_MS);
    if (res != ESP_OK) return res;

    time.tm_sec = bcd2dec(data[0]);
    time.tm_min = bcd2dec(data[1]);
    if (data[2] & DS3231_12HOUR_FLAG) {
        time.tm_hour = bcd2dec(data[2] & DS3231_12HOUR_MASK) - 1;
        if (data[2] & DS3231_PM_FLAG) time.tm_hour += 12;
    } else {
        time.tm_hour = bcd2dec(data[2]);
    }
    time.tm_wday = bcd2dec(data[3]) - 1;
    time.tm_mday = bcd2dec(data[4]);
    time.tm_mon  = bcd2dec(data[5] & DS3231_MONTH_MASK) - 1;
    time.tm_year = bcd2dec(data[6]) + 100;
    time.tm_isdst = 0;
    return ESP_OK;
}

esp_err_t DS3231::getRawTemp(int16_t& temp) {
    uint8_t reg = DS3231_ADDR_TEMP;
    uint8_t data[2];
    esp_err_t res = i2c_master_transmit_receive(i2c_devHandle, &reg, 1, data, 2, I2CDEV_TIMEOUT / portTICK_PERIOD_MS);
    if (res == ESP_OK)
        temp = (int16_t)(int8_t)data[0] << 2 | data[1] >> 6;
    return res;
}

esp_err_t DS3231::getTempInteger(int8_t& temp) {
    int16_t t_int;
    esp_err_t res = getRawTemp(t_int);
    if (res == ESP_OK)
        temp = t_int >> 2;
    return res;
}

esp_err_t DS3231::getTempFloat(float& temp) {
    int16_t t_int;
    esp_err_t res = getRawTemp(t_int);
    if (res == ESP_OK)
        temp = t_int * 0.25f;
    return res;
}