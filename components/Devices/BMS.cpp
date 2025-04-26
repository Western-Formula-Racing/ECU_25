#include "BMS.h"
#include "esp_log.h"
static const char *TAG = "BMS"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

BMS *BMS::instancePtr = nullptr;
SemaphoreHandle_t BMS::mutex = xSemaphoreCreateMutex();

BMS::BMS()
{
    //get these from storage later
    max_discharge_current = 5;
    max_charge_current = 0; 
    
    BMS_Max_Discharge_Current_ID514.set(max_discharge_current);
    BMS_Max_Charge_Current_ID514.set(max_charge_current);
    ESP_LOGI(TAG, "BMS Initialized");
}

BMS *BMS::Get()
{
    if (instancePtr == nullptr && BMS::mutex)
    {
        if (xSemaphoreTake(BMS::mutex, (TickType_t)10) == pdTRUE)
        {
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

BMS::STATE BMS::getPackState()
{
    return (STATE)PackStatus_ID1056.get_int();
}