#include "Sensors.h"
#include "esp_log.h"
static const char *TAG = "Sensors"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

Sensors *Sensors::instancePtr = nullptr;
SemaphoreHandle_t Sensors::mutex = xSemaphoreCreateMutex();

Sensors::Sensors()
{
    ESP_LOGI(TAG, "Sensors Initialized");
}

Sensors *Sensors::Get()
{
    if (instancePtr == nullptr && Sensors::mutex)
    {
        if (xSemaphoreTake(Sensors::mutex, (TickType_t)10) == pdTRUE)
        {
            instancePtr = new Sensors();
            xSemaphoreGive(Sensors::mutex);
        }
        else
        {
            ESP_LOGW(TAG, "Mutex couldn't be obtained");
        }
    }
    return instancePtr;
}