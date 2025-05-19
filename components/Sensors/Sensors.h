#ifndef _SENSORS_
#define _SENSORS_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "IO.h"
#include "etl/array.h"
#include "CAN_Config.hpp"

class Sensors{
private:
    //Singleton device class structure
    static Sensors* instancePtr;
    static SemaphoreHandle_t mutex;
    Sensors();
    etl::array<float, 14> sensor_voltages;

public:
    typedef enum{
        APPS1 = ECU_9_A9,
        APPS2 = ECU_17_A14,
        BPS1 = ECU_3_A3,
        BPS2 = ECU_4_A4,
    }SENSOR_INDEX;
    //Deleting the copy constructor and copy reference constructor to prevent copies
    Sensors(const Sensors &) = delete;
    Sensors &operator=(const Sensors &) = delete;
    Sensors(Sensors &&) = delete;
    Sensors &operator=(Sensors &&) = delete;
    static Sensors* Get();
    void poll_sensors();
    etl::array<float,14> get_sensor_voltages();
    float get_sensor_voltage(SENSOR_INDEX index);

    

};

#endif