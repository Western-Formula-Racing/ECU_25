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
#include "Pedals.h"

static const char* TAG = "Main"; //Used for ESP_LOGx commands. See ESP-IDF Documentation
CAN can0{GPIO_NUM_16, GPIO_NUM_15, TWAI_MODE_NORMAL};


// must extern C since this gets called by a C file for freeRTOS
extern "C" void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGI(TAG, "Main Has begun");
    can0.begin();
    
    // IO::Get();// setup IO for heart beat
    // xTaskCreatePinnedToCore(StateMachine::StateMachineLoop, "StateMachineLoop", 4096, NULL, configMAX_PRIORITIES - 1, nullptr, 1);
    bool onboard_LED = 0;
    while(true){
        onboard_LED = !onboard_LED;
        // printf(">adc2_channel_0:%2.f\n", IO::Get()->adc2_handle->readVoltage(0));
        // printf(">A9:%2.f\n", IO::Get()->analogReadVoltage(ECU_9_A9));
        // printf(">adc2_channel_0:%2.f\n", IO::Get()->adc2_handle->readVoltage(0));
        // printf(">A9:%2.f\n", IO::Get()->analogReadVoltage(ECU_9_A9));
        // for (int i = 0; i<=7; i++){
        //     printf(">adc1_%d:%.2f\n", i, IO::Get()->adc1_handle->readVoltage(i));
        // }
        for (int i = 0; i<=13; i++){
            printf(">A%d:%.2f\n", i, IO::Get()->analogReadVoltage(i));
        }
        for (int i = 0; i<=7; i++){
            printf(">adc2_%d:%.2f\n", i, IO::Get()->adc2_handle->readVoltage(i));
        }
        
        


        gpio_set_level(GPIO_NUM_48, onboard_LED);  // heart beat LED  
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
}