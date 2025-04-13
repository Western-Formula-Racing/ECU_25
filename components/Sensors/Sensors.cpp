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

void Sensors::poll_sensors()
{

for (int i = 0; i<=7; i++){
        float value = IO::Get()->adc1_handle->readVoltage(i);
        sensor_voltages[i] = value;
        printf(">adc1_%d:%.2f\n", i, value);
    }
    for (int i = 0; i<=5; i++){
        float value = IO::Get()->adc2_handle->readVoltage(i);
        sensor_voltages[i+8] = value;
        printf(">adc2_%d:%.2f\n", i, value);
    }

    // for (int i = 0; i <= 13; i++){
        
    //     sensor_voltages[i] = IO::Get()->analogReadVoltage(static_cast<ECU_ANALOG_PIN>(i));
    //     printf(">ADC%d_voltage:%.2f\n", i+1, sensor_voltages[i]);
    // }
}

etl::array<float,14> Sensors::get_sensor_voltages(){
    return sensor_voltages;
}