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
    for (int i = 0; i <= 13; i++){
        sensor_voltages[i] = IO::Get()->analogReadVoltage(static_cast<ECU_ANALOG_PIN>(i));
        printf(">ADC%d_voltage:%.2f\n", i+1, sensor_voltages[i]);
    }
    A1_ID2014.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(0)));
    A2_ID2014.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(1)));
    A3_ID2014.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(2)));
    A4_ID2014.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(3)));
    A5_ID2015.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(4)));
    A6_ID2015.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(5)));
    A7_ID2015.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(6)));
    A8_ID2015.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(7)));
    A9_ID2016.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(8)));
    A10_ID2016.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(9)));
    A11_ID2016.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(10)));
    A12_ID2016.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(11)));
    A13_ID2017.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(12)));
}

etl::array<float,14> Sensors::get_sensor_voltages(){
    return sensor_voltages;
}

float Sensors::get_sensor_voltage(SENSOR_INDEX index)
{
    if(index < sensor_voltages.max_size()){
        return sensor_voltages[index];
    }
    return -1;
}