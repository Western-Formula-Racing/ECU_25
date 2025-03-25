#ifndef _IO_
#define _IO_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "driver/i2c_master.h"
#include "PinMap.h"
#include "TLA2518.h"
#include "driver/gpio.h"
#include "FXL6408UMX.h"
#include "ICM-20948.h"

class IO{
private:
    //Singleton device class structure
    IO();
    static IO* instancePtr;
    static SemaphoreHandle_t mutex;
    TLA2518* adc1_handle;
    TLA2518* adc2_handle;
    i2c_master_bus_handle_t i2c_handle;
    FXL6408UMX* hsd_handle;
    ICM20948* imu_handle;
    public:
    
    //Deleting the copy constructor and copy reference constructor to prevent copies
    IO(const IO &) = delete;
    IO &operator=(const IO &) = delete;
    IO(IO &&) = delete;
    IO &operator=(IO &&) = delete;
    static IO* Get();
    void setupSPI();
    int analogRead(ECU_ANALOG_PIN pin);
    double analogReadVoltage(ECU_ANALOG_PIN pin);
    void digitalRead(ECU_IO_PIN pin);
    void digitalWrite(ECU_IO_PIN pin);    void setupI2C();
    void HSDWrite(ECU_HSD_PIN channel, bool level);
    double getAccelX();
    double getAccelY();
    double getAccelZ();
    double getGyroX();
    double getGyroY();
    double getGyroZ();

};
#endif