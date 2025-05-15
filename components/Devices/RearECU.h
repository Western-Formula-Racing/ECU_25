#ifndef _REARECU_
#define _REARECU_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


class RearECU{
private:
    //Singleton device class structure
    static RearECU* instancePtr;
    static SemaphoreHandle_t mutex;
    RearECU();
public:
    //Deleting the copy constructor and copy reference constructor to prevent copies
    RearECU(const RearECU &) = delete;
    RearECU &operator=(const RearECU &) = delete;
    RearECU(RearECU &&) = delete;
    RearECU &operator=(RearECU &&) = delete;
    static RearECU* Get();

};

#endif