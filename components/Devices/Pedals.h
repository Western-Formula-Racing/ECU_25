#ifndef _PEDALS_
#define _PEDALS_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "IO.h"
#include "driver/gpio.h"


class Pedals{
private:
    //Singleton device class structure
    static Pedals* instancePtr;
    static SemaphoreHandle_t mutex;
    Pedals();
public:
    //Deleting the copy constructor and copy reference constructor to prevent copies
    Pedals(const Pedals &) = delete;
    Pedals &operator=(const Pedals &) = delete;
    Pedals(Pedals &&) = delete;
    Pedals &operator=(Pedals &&) = delete;
    static Pedals* Get();

};

#endif