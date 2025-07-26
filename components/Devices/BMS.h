/**
 * @file BMS.h
 * @author Yamaan Bakir
 * @brief Declares the BMS class, defines variables and methods we need to store and/or use from the BMS such as pack state and current limits.
 * @version 0.1
 * @date 2025-07-10
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#ifndef _BMS_
#define _BMS_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "CAN_Config.hpp"

/**
 * @brief Stores all the data we collect from the BMS, and 
 * defines the functions we will use to share some of that data.
 */
class BMS
{
private:
    // Singleton device class structure
    static BMS *instancePtr;
    static SemaphoreHandle_t mutex;
    BMS();
    int max_discharge_current;
    int max_charge_current;

public:
    // Deleting the copy constructor and copy reference constructor to prevent copies
    BMS(const BMS &) = delete;
    BMS &operator=(const BMS &) = delete;
    BMS(BMS &&) = delete;
    BMS &operator=(BMS &&) = delete;
    static BMS *Get();


    enum STATE
    {
        IDLE,
        PRECHARGE_START,
        PRECHARGING,
        ACTIVE,
        CHARGING,
        CHARGE_COMPLETE,
        FAULT
    };

    int getPackVoltage();
    STATE getPackState();
};

#endif