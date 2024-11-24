#ifndef _INVERTER_
#define _INVERTER_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


class Inverter{
private:
    //Singleton device class structure
    static Inverter* instancePtr;
    static SemaphoreHandle_t mutex;
    Inverter();
public:
    //Deleting the copy constructor and copy reference constructor to prevent copies
    Inverter(const Inverter &) = delete;
    Inverter &operator=(const Inverter &) = delete;
    Inverter(Inverter &&) = delete;
    Inverter &operator=(Inverter &&) = delete;
    static Inverter* Get();


};

#endif