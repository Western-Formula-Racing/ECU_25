#ifndef _PEDALS_
#define _PEDALS_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "IO.h"
#include "driver/gpio.h"
#include "CAN_Config.hpp"
#include "Sensors.h"

#define BRAKE_PRESSURE_FACTOR 1.0f
#define BRAKE_PRESSURE_OFFSET 0.0f
#define BRAKE_THRESHOLD 2.1f
#define APPS_PLAUSABILITY_THRESHOLD 0.1f

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

class Pedals
{
private:
    // Singleton device class structure
    static Pedals *instancePtr;
    static SemaphoreHandle_t mutex;
    Pedals();
    // Calibration values
    float apps1_min_voltage;
    float apps1_max_voltage;
    float apps2_min_voltage;
    float apps2_max_voltage;
    float apps1_voltage;
    float apps2_voltage;
    float apps_sensor1_percent;
    float apps_sensor2_percent;
    float throttle;
    float brakePressure_front;
    float brakePressure_rear;

    bool fault_latch;

public:
    // Deleting the copy constructor and copy reference constructor to prevent copies
    Pedals(const Pedals &) = delete;
    Pedals &operator=(const Pedals &) = delete;
    Pedals(Pedals &&) = delete;
    Pedals &operator=(Pedals &&) = delete;
    static Pedals *Get();
    float getThrottle();
    float getBrakePressure();
    void updateAppsCalibration(float app1_min, float app1_max, float app2_min, float app2_max);

};

#endif