#ifndef MAIN_DS3231_H_
#define MAIN_DS3231_H_

#include <time.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "driver/i2c_master.h"
#include "esp_err.h"

#define DS3231_ADDR 0x68 //!< I2C address

#define DS3231_STAT_OSCILLATOR 0x80
#define DS3231_STAT_32KHZ      0x08
#define DS3231_STAT_BUSY       0x04
#define DS3231_STAT_ALARM_2    0x02
#define DS3231_STAT_ALARM_1    0x01

#define DS3231_CTRL_OSCILLATOR    0x80
#define DS3231_CTRL_SQUAREWAVE_BB 0x40
#define DS3231_CTRL_TEMPCONV      0x20
#define DS3231_CTRL_ALARM_INTS    0x04
#define DS3231_CTRL_ALARM2_INT    0x02
#define DS3231_CTRL_ALARM1_INT    0x01

#define DS3231_ALARM_WDAY   0x40
#define DS3231_ALARM_NOTSET 0x80

#define DS3231_ADDR_TIME    0x00
#define DS3231_ADDR_ALARM1  0x07
#define DS3231_ADDR_ALARM2  0x0b
#define DS3231_ADDR_CONTROL 0x0e
#define DS3231_ADDR_STATUS  0x0f
#define DS3231_ADDR_AGING   0x10
#define DS3231_ADDR_TEMP    0x11

#define DS3231_12HOUR_FLAG  0x40
#define DS3231_12HOUR_MASK  0x1f
#define DS3231_PM_FLAG      0x20
#define DS3231_MONTH_MASK   0x1f


class DS3231 {
public:
    DS3231(i2c_master_bus_handle_t busHandle, uint8_t address = 0x68, uint32_t clk_speed = 50000);

    esp_err_t setTime(const struct tm& time);
    esp_err_t getTime(struct tm& time);
    esp_err_t getRawTemp(int16_t& temp);
    esp_err_t getTempInteger(int8_t& temp);
    esp_err_t getTempFloat(float& temp);

private:
    i2c_master_bus_handle_t i2c_busHandle;
    i2c_master_dev_handle_t i2c_devHandle;
    uint8_t dev_addr;
    uint32_t clk_speed;

    uint8_t bcd2dec(uint8_t val) const;
    uint8_t dec2bcd(uint8_t val) const;
};

#endif /* MAIN_DS3231_H_ */

