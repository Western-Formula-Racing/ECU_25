#include "Inverter.h"
#include "esp_log.h"
static const char *TAG = "Inverter"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

Inverter *Inverter::instancePtr = nullptr;
SemaphoreHandle_t Inverter::mutex = xSemaphoreCreateMutex();

Inverter::Inverter()
{
    ESP_LOGI(TAG, "Inverter Initialized");
}

Inverter *Inverter::Get()
{
    if (instancePtr == nullptr && Inverter::mutex)
    {
        if (xSemaphoreTake(Inverter::mutex, (TickType_t)10) == pdTRUE)
        {
            instancePtr = new Inverter();
            xSemaphoreGive(Inverter::mutex);
        }
        else
        {
            ESP_LOGW(TAG, "Mutex couldn't be obtained");
        }
    }
    return instancePtr;
}