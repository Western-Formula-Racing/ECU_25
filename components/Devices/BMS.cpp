/**
 * @file BMS.cpp
 * @author Yamaan Bakir
 * @brief Handles logic and communicates with the the BMS.
 * @version 0.1
 * @date 2025-07-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "BMS.h"
#include "esp_log.h"
static const char *TAG = "BMS"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

BMS *BMS::instancePtr = nullptr;
SemaphoreHandle_t BMS::mutex = xSemaphoreCreateMutex();

/**
 * @brief Construct a new BMS::BMS object
 * 
 */
BMS::BMS()
{
    //get these from storage later
    max_discharge_current = 5;
    max_charge_current = 0; 
    
    BMS_Max_Discharge_Current_ID514.set(max_discharge_current);
    BMS_Max_Charge_Current_ID514.set(max_charge_current);
    ESP_LOGI(TAG, "BMS Initialized");
}

/**
 * @brief This method ensures only one instance of BMS exists by checking and creating it 
 * if needed. It uses mutex to prevent race conditions during instantiation.
 *
 * @return BMS* Pointer to the BMS instance.
 */

BMS *BMS::Get()
{
    //If the doesnt have a BMS instance and the lock as created
    if (instancePtr == nullptr && BMS::mutex)
    {
        //Try to take the mutex 
        if (xSemaphoreTake(BMS::mutex, (TickType_t)10) == pdTRUE)
        {
            //Create a BMS instance and release mutex
            instancePtr = new BMS();
            xSemaphoreGive(BMS::mutex);
        }
        else
        {
            ESP_LOGW(TAG, "Mutex couldn't be obtained");
        }
    }
    return instancePtr;
}

/**
 * @brief Lets us know what state the pack is in, the various states were defined earlier in enum STATE.
 * 
 * @return BMS::STATE 
 */
BMS::STATE BMS::getPackState()
{
    return (STATE)PackStatus_ID1056.get_int();
}