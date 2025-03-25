#ifndef _IO_
#define _IO_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "PinMap.h"
#include "TLA2518.h"
#include "ICM-20948.h"

class IO{
private:
    //Singleton device class structure
    IO();
    static IO* instancePtr;
    static SemaphoreHandle_t mutex;
    TLA2518 adc1;
    TLA2518 adc2;
    public:
    ICM20948 imu;
    //Deleting the copy constructor and copy reference constructor to prevent copies
    IO(const IO &) = delete;
    IO &operator=(const IO &) = delete;
    IO(IO &&) = delete;
    IO &operator=(IO &&) = delete;
    static IO* Get();
    void setupSPI();
    int analogRead(analogInputChannel channel);
    double analogReadVoltage(analogInputChannel channel);

};
#endif