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

float Pedals::getThrottle()
{
    throttle = 0;
    // Temporary test values
    apps1_min_voltage = 0.0f;
    apps2_min_voltage = 0.0f;
    apps1_max_voltage = 2.0f;
    apps2_max_voltage = 2.0f;
    float apps1_voltage = apps1_voltage_signal.get_float();
    float apps2_voltage = apps2_voltage_signal.get_float();
    
    apps_sensor1_percent = min(1, max(0, (apps1_voltage - apps1_min_voltage) / (apps1_max_voltage - apps1_min_voltage)));
    apps_sensor2_percent = min(1, max(0, (apps2_voltage - apps2_min_voltage) / (apps2_max_voltage - apps2_min_voltage)));
    throttle = (apps_sensor1_percent + apps_sensor2_percent)/2;
    // Case 1: sensor open circuit
    if(apps1_voltage <= 0.01f || apps2_voltage <= 0.01f)
    {
        fault_latch = true;
        throttle = 0;
    }

    // Case 2: pedal implausability
    else if(max(apps_sensor1_percent, apps_sensor2_percent) - min(apps_sensor1_percent, apps_sensor2_percent))
    {
        fault_latch = true;
        throttle = 0;
    }

    // Case 3: clear faults
    else if(throttle <= 0.05f)
    {
        fault_latch = false;
    }


    if(fault_latch){
        throttle = 0;
    }

    return throttle;

}