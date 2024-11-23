#ifndef _BMS_
#define _BMS_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


class BMS{
private:
    //Singleton device class structure
    static BMS* instancePtr;
    static SemaphoreHandle_t mutex;
    BMS();
public:
    //Deleting the copy constructor and copy reference constructor to prevent copies
    BMS(const BMS &) = delete;
    BMS &operator=(const BMS &) = delete;
    BMS(BMS &&) = delete;
    BMS &operator=(BMS &&) = delete;
    static BMS* Get();

};

#endif