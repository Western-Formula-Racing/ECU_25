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

    enum CalibState {
        CALIB_IDLE,
        CALIB_WAIT,
        CALIB_SAMPLING
    };

    enum CalibTarget {
        TARGET_NONE,
        TARGET_MIN,
        TARGET_MAX
    };

    inline CalibState calib_state = CALIB_IDLE;
    inline CalibTarget calib_target = TARGET_NONE;
    inline int64_t calib_timer = 0;
    inline etl::vector<float, 300> calib_samples_apps1; // 10ms * 300 = 3s
    inline etl::vector<float, 300> calib_samples_apps2;
}
#endif