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
    apps1_min_voltage = 2.5f;
    apps2_min_voltage = 2.5f;
    apps1_max_voltage = 1.0f;
    apps2_max_voltage = 4.0f;
    apps1_voltage = Sensors::Get()->get_sensor_voltage(Sensors::APPS1);
    apps2_voltage = Sensors::Get()->get_sensor_voltage(Sensors::APPS2);
    printf(">sensor1:%.2f\n>sensor2:%.2f\n", apps1_voltage, apps2_voltage);
    
    apps_sensor1_percent = min(1, max(0, (apps1_voltage - apps1_min_voltage) / (apps1_max_voltage - apps1_min_voltage)));
    apps_sensor2_percent = min(1, max(0, (apps2_voltage - apps2_min_voltage) / (apps2_max_voltage - apps2_min_voltage)));
    throttle = (apps_sensor1_percent + apps_sensor2_percent)/2;
    printf(">sensor1_percent:%.2f\n>sensor2_percent:%.2f\n", apps_sensor1_percent, apps_sensor2_percent);
    printf(">fault:%d\n", fault_latch);
    // Case 1: sensor open circuit
    if(apps1_voltage <= 0.01f || apps2_voltage <= 0.01f)
    {
        ESP_LOGW(TAG, "open circuit\n");
        fault_latch = true;
        throttle = 0;
    }

    // Case 2: pedal implausability
    else if((max(apps_sensor1_percent, apps_sensor2_percent) - min(apps_sensor1_percent, apps_sensor2_percent)) >= BRAKE_PLAUSABILITY_THRESHOLD)
    {
        ESP_LOGW(TAG, "implausability\n");
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

float Pedals::getBrakePressure()
{
    return Sensors::Get()->get_sensor_voltage(Sensors::BPS1);
}