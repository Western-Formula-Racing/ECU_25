#ifndef _SENSORS_
#define _SENSORS_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


class Sensors{
private:
    //Singleton device class structure
    static Sensors* instancePtr;
    static SemaphoreHandle_t mutex;
    Sensors();
public:
    //Deleting the copy constructor and copy reference constructor to prevent copies
    Sensors(const Sensors &) = delete;
    Sensors &operator=(const Sensors &) = delete;
    Sensors(Sensors &&) = delete;
    Sensors &operator=(Sensors &&) = delete;
    static Sensors* Get();

};

#endif