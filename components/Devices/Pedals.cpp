#include "Pedals.h"
#include "esp_log.h"
static const char *TAG = "Pedals"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

Pedals *Pedals::instancePtr = nullptr;
SemaphoreHandle_t Pedals::mutex = xSemaphoreCreateMutex();

Pedals::Pedals()
{
    // Temporary test values
    apps1_min_voltage = 2.64f;
    apps2_min_voltage = 2.32f;
    apps1_max_voltage = 3.44f;
    apps2_max_voltage = 1.52f;
    fault_latch = 0;
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
    apps1_voltage = Sensors::Get()->get_sensor_voltage(Sensors::APPS1);
    apps2_voltage = Sensors::Get()->get_sensor_voltage(Sensors::APPS2);
    printf(">sensor1:%.2f\n>sensor2:%.2f\n", apps1_voltage, apps2_voltage);

    apps_sensor1_percent = min(1, max(0, (apps1_voltage - apps1_min_voltage) / (apps1_max_voltage - apps1_min_voltage)));
    apps_sensor2_percent = min(1, max(0, (apps2_voltage - apps2_min_voltage) / (apps2_max_voltage - apps2_min_voltage)));
    throttle = (apps_sensor1_percent + apps_sensor2_percent) / 2;
    printf(">apps1_min:%.2f\n>apps1_max:%.2f\n", apps1_min_voltage, apps1_max_voltage);
    printf(">apps2_min:%.2f\n>apps2_max:%.2f\n", apps2_min_voltage, apps1_max_voltage);
    printf(">sensor1_percent:%.2f\n>sensor2_percent:%.2f\n", apps_sensor1_percent, apps_sensor2_percent);
    printf(">fault:%d\n", fault_latch);
    // Case 1: sensor open circuit
    if (apps1_voltage <= 0.01f || apps2_voltage <= 0.01f)
    {
        ESP_LOGW(TAG, "open circuit\n");
        fault_latch = true;
        throttle = 0;
    }

    // Case 2: pedal implausability
    else if ((max(apps_sensor1_percent, apps_sensor2_percent) - min(apps_sensor1_percent, apps_sensor2_percent)) >= APPS_PLAUSABILITY_THRESHOLD)
    {
        ESP_LOGW(TAG, "implausability\n");
        fault_latch = true;
        throttle = 0;
    }
    // Case 3: brakes pressed
    else if (getBrakePressure() >= BRAKE_PLAUSABILITY_THRESHOLD)
    {
        ESP_LOGW(TAG, "brake pressed\n");
        fault_latch = true;
        throttle = 0;
    }

    // clear faults
    else if (throttle <= 0.08f)
    {
        fault_latch = 0;
    }

    if (fault_latch)
    {
        throttle = 0;
    }

    return throttle;
}

float Pedals::getBrakePressure()
{
    return (Sensors::Get()->get_sensor_voltage(Sensors::BPS1) + Sensors::Get()->get_sensor_voltage(Sensors::BPS2)) / 2;
}

void Pedals::updateAppsCalibration(float app1_min, float app1_max, float app2_min, float app2_max)
{
    if (app1_min != apps1_min_voltage)
    {
        apps1_min_voltage = app1_min;
    }
    if (app1_max != apps1_max_voltage)
    {
        apps1_max_voltage = app1_max;
    }
    if (app2_min != apps2_min_voltage)
    {
        apps2_min_voltage = app2_min;
    }
    if (app2_max != apps1_max_voltage)
    {
        apps2_max_voltage = app2_max;
    }
}

void Pedals::set_min(float apps1, float apps2)
{
    apps1_min_voltage = apps1;
    apps2_min_voltage = apps2;
}
void Pedals::set_max(float apps1, float apps2)
{
    apps1_max_voltage = apps1;
    apps2_max_voltage = apps2;
}
float Pedals::get_apps1_voltage()
{
    return apps1_voltage;
}
float Pedals::get_apps2_voltage()
{
    return apps2_voltage;
}