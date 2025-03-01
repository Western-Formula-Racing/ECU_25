#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "CAN.h"
#include "Sensors.h"
#include "IO.h"
#include "Devices.h"
#include "esp_log.h"
#include "CAN.h"
#include "driver/twai.h"
#include "CAN_Config.hpp"
#include "StateMachine.hpp"

static const char* TAG = "Main"; //Used for ESP_LOGx commands. See ESP-IDF Documentation
CAN can0{GPIO_NUM_47, GPIO_NUM_48, TWAI_MODE_NORMAL};


// must extern C since this gets called by a C file for freeRTOS
extern "C" void app_main(void)
{
    // vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGI(TAG, "Main Has begun");
    can0.begin();
    
    float torque = 100.0f;
    int directionCommand = 1;
    bool inverterEnable = false;
    bool inverterDischarge = false;
    bool speedModeEnable = false;
    float torqueLimit = 200.0f;
    xTaskCreatePinnedToCore(StateMachine::StateMachineLoop, "StateMachineLoop", 4096, NULL, configMAX_PRIORITIES - 1, nullptr, 1);
    while(true){
        torque = torque *-1;
        torqueRequest_Signal.set(torque);
        directionCommand_Signal.set(directionCommand);
        inverterEnable_Signal.set(directionCommand);
        inverterDischarge_Signal.set(inverterDischarge);
        speedModeEnable_Signal.set(speedModeEnable);
        torqueLimit_Signal.set(torqueLimit);
        inverterEnable_Signal.set(inverterEnable);
        // printf("inverter torque req raw: %lld\n", torqueRequest_Signal.get_raw());
        // printf("inverter torque req uint: %lld\n", torqueRequest_Signal.get_uint64());
        // printf("inverter torque req int: %d\n", torqueRequest_Signal.get_int());
        // printf("inverter torque req bool: %d\n", torqueRequest_Signal.get_bool());
        // printf("inverter torque req float: %f\n", torqueRequest_Signal.get_float());
        printf("heart beat!\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
}