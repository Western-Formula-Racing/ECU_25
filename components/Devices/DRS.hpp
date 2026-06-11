#pragma once
#include "driver/twai.h"

namespace DRS {
    // Servo position values — calibrate to your actual wing travel
    constexpr uint16_t POSITION_OPEN   = 10922;  // REG_POSITION_MAX_LIMIT default
    constexpr uint16_t POSITION_CLOSED = 5462;   // REG_POSITION_MIN_LIMIT default

    void init();
    void update();  // call this in the RearECU loop
}