#ifndef _REARECU_
#define _REARECU_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "CAN.h"
#include "CAN_Config.hpp"
#include "IO.h"
#include "Sensors.h"

namespace RearECU{

    void rearECU_Task(void *);

};

#endif