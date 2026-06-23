#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "etl/array.h"
#include "CAN_Config.hpp"
#include "IO.h"
#include "Pedals.h"
#include "Sensors.h"
#include "Inverter.h"
#include "BMS.h"
#include "nvs_flash.h"
#include "nvs.h"

#define RTD_TIME 2000
#define BRAKE_LIGHT_THRESHOLD 0.7f

// Autonomous demo constants
#define AUTONOMOUS_ARM_WINDOW_MS  10000   // 10s window after entering DRIVE to arm
#define AUTONOMOUS_ARM_HOLD_MS    3000    // hold RTD for 3s within window to trigger
#define AUTONOMOUS_TOTAL_MS       15000   // 15 seconds total
#define AUTONOMOUS_TORQUE_1       0.10f   // 10% torque
#define AUTONOMOUS_TORQUE_2       0.20f   // 20% torque
#define AUTONOMOUS_PHASE1_MS      5000    // 0–5s:  10%
#define AUTONOMOUS_PHASE2_MS      10000   // 5–10s: 20%
#define AUTONOMOUS_PHASE3_MS      15000   // 10–15s: 10%
#define AUTONOMOUS_BRAKE_THRESH   0.10f   // interrupt if brake > 10% travel
#define AUTONOMOUS_THROTTLE_THRESH 0.10f  // interrupt if throttle > 10%
// Blink-blink-stop RTD pattern (Airbus strobe style)
// 125ms ON, 125ms OFF, 125ms ON, 625ms OFF = 1s cycle
#define AUTONOMOUS_BLINK_TICK_MS  125
#define AUTONOMOUS_BLINK_CYCLE_MS 1000
namespace StateMachine{
enum State
{
    START,
    PRECHARGE_ENABLE,
    PRECHARGE_OK,
    STARTUP_DELAY,
    DRIVE,
    PRECHARGE_ERROR,
    DEVICE_FAULT,
};


// Creating a type for state function pointers
typedef State (*state_function_t)();


void StateMachineLoop(void *);
State handle_start();
State handle_precharge_enable();
State handle_precharge_ok();
State handle_startup_delay();
State handle_drive();
State handle_precharge_error();
State handle_device_fault();
void setupAppsCalibration();
void checkNewAppsCalibration();

}
#endif