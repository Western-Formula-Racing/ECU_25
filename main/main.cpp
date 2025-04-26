#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "CAN.h"
#include "Sensors.h"
#include "IO.h"
#include "Devices.h"
#include "GUI.h"
#include "esp_log.h"
#include "CAN.h"
#include "driver/twai.h"
#include "CAN_Config.hpp"
#include "StateMachine.hpp"
#include "Pedals.h"
#include "RearECU.h"
#include "GUI.h"
#define MAIN_DELAY 600

static const char* TAG = "Main"; //Used for ESP_LOGx commands. See ESP-IDF Documentation
CAN can0{GPIO_NUM_16, GPIO_NUM_15, TWAI_MODE_NORMAL};


// must extern C since this gets called by a C file for freeRTOS
extern "C" void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGI(TAG, "Main Has begun");
    can0.begin();
    if(IO::Get()->digitalRead(ECU_SELECT)){
        //Front ECU
        printf("FRONT ECU SELECTED!\n");
        xTaskCreatePinnedToCore(StateMachine::StateMachineLoop, "StateMachineLoop", 4096, NULL, configMAX_PRIORITIES - 1, nullptr, 1);
    }
    else{
        //Rear ECU
        printf("REAR ECU SELECTED!\n");
        xTaskCreatePinnedToCore(RearECU::rearECU_Task, "RearECULoop", 4096, NULL, configMAX_PRIORITIES - 1, nullptr, 1);
    }
    
    bool onboard_LED = 0;
    while(true){
        onboard_LED = !onboard_LED;
        gpio_set_level(GPIO_NUM_48, onboard_LED);  // heart beat LED  
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));
    }
}