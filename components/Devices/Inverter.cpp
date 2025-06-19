#include "Inverter.h"
#include "esp_log.h"
static const char *TAG = "Inverter"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

Inverter *Inverter::instancePtr = nullptr;
SemaphoreHandle_t Inverter::mutex = xSemaphoreCreateMutex();

Inverter::Inverter()
{
    torqueLimit = 230; // get this from memory later
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

void Inverter::Disable()
{
    VCU_INV_Torque_Command_ID192.set(0);
    VCU_INV_Inverter_Enable_ID192.set(false);
    VCU_INV_Torque_Limit_Command_ID192.set(0);
}

void Inverter::Enable()
{
    VCU_INV_Inverter_Enable_ID192.set(true);
    VCU_INV_Torque_Limit_Command_ID192.set(torqueLimit);
}

void Inverter::SetTorqueRequest(float throttle)
{
    VCU_INV_Torque_Command_ID192.set(throttle*torqueLimit);
}