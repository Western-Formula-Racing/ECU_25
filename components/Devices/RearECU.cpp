#include "RearECU.h"
#include "esp_log.h"
static const char* TAG = "RearECU"; //Used for ESP_LOGx commands. See ESP-IDF Documentation

RearECU *RearECU::instancePtr = nullptr;
SemaphoreHandle_t RearECU::mutex = xSemaphoreCreateMutex();

RearECU::RearECU()
{
    ESP_LOGI(TAG, "RearECU Initialized");
}

RearECU *RearECU::Get()
{
    if (instancePtr == nullptr && RearECU::mutex)
    {
        if (xSemaphoreTake(RearECU::mutex, (TickType_t)10) == pdTRUE)
        {
            instancePtr = new RearECU();
            xSemaphoreGive(RearECU::mutex);
        }
        else
        {
            ESP_LOGW(TAG, "Mutex couldn't be obtained");
        }
    }
    return instancePtr;
}