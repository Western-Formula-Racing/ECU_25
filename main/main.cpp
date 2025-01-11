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

static const char* TAG = "Main"; //Used for ESP_LOGx commands. See ESP-IDF Documentation
CAN can0{GPIO_NUM_47, GPIO_NUM_48, TWAI_MODE_NORMAL};


// must extern C since this gets called by a C file for freeRTOS
extern "C" void app_main(void)
{
    // vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGI(TAG, "Main Has begun");
    can0.begin();
    

    while(true){
        // printf("inverter torque req raw: %lld\n", torqueRequest_Signal.get_raw());
        // printf("inverter torque req uint: %lld\n", torqueRequest_Signal.get_uint64());
        // printf("inverter torque req int: %d\n", torqueRequest_Signal.get_int());
        // printf("inverter torque req bool: %d\n", torqueRequest_Signal.get_bool());
        // printf("inverter torque req float: %f\n", torqueRequest_Signal.get_float());
        printf("heart beat!\n");
        vTaskDelay(100);
    }
    
}