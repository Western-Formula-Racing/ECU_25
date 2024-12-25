#include "BMS.h"
#include "esp_log.h"
static const char *TAG = "BMS"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

BMS *BMS::instancePtr = nullptr;
SemaphoreHandle_t BMS::mutex = xSemaphoreCreateMutex();

BMS::BMS()
{
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