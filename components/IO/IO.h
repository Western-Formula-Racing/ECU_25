#ifndef _IO_
#define _IO_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "PinMap.h"
#include "TLA2518.h"


class IO{
private:
    //Singleton device class structure
    static IO* instancePtr;
    static SemaphoreHandle_t mutex;
    spi_device_handle_t adc1Handle;
    spi_device_handle_t adc2Handle;
  public:
    //Deleting the copy constructor and copy reference constructor to prevent copies
    IO();
    IO(const IO &) = delete;
    IO &operator=(const IO &) = delete;
    IO(IO &&) = delete;
    IO &operator=(IO &&) = delete;
    static IO* Get();
    void setupSPI();

};
#endif