#include "Pedals.h"
#include "esp_log.h"
static const char *TAG = "Pedals"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

Pedals *Pedals::instancePtr = nullptr;
SemaphoreHandle_t Pedals::mutex = xSemaphoreCreateMutex();

Pedals::Pedals()
{
    ESP_LOGI(TAG, "Pedals Initialized");
}

Pedals *Pedals::Get()
{
    if (instancePtr == nullptr && Pedals::mutex)
    {
        if (xSemaphoreTake(Pedals::mutex, (TickType_t)10) == pdTRUE)
        {
            instancePtr = new Pedals();
            xSemaphoreGive(Pedals::mutex);
        }
        else
        {
            ESP_LOGW(TAG, "Mutex couldn't be obtained");
        }
    }
    return instancePtr;
}