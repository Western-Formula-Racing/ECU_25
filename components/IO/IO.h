#ifndef _IO_
#define _IO_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "PinMap.h"

#define ADC_CMD_READ    0b00010000
#define ADC_CMD_WRITE   0b00001000


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
    void Setup();

};

void io_task(void *pvParameters);
void adcSetup(spi_device_handle_t devHandle);
void adcRead(spi_device_handle_t devHandle, uint8_t channel ,uint8_t *rx_buffer);
#endif