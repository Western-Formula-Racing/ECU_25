#ifndef _BMS_
#define _BMS_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "CAN_Config.hpp"

class BMS{
private:
    //Singleton device class structure
    static BMS* instancePtr;
    static SemaphoreHandle_t mutex;
    BMS();
    int max_discharge_current;
    int max_charge_current;
public:
    //Deleting the copy constructor and copy reference constructor to prevent copies
    BMS(const BMS &) = delete;
    BMS &operator=(const BMS &) = delete;
    BMS(BMS &&) = delete;
    BMS &operator=(BMS &&) = delete;
    static BMS* Get();

    enum STATE
    {
        IDLE,
        PRECHARGE,
        ACTIVE,
        CHARGING,
        FAULT
    };

    int getPackVoltage();
    STATE getPackState();

};

#endif