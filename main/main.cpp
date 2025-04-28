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
#include "ds3231.h"
#include "Logger.h"
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
        Logger::init();
        xTaskCreatePinnedToCore(RearECU::rearECU_Task, "RearECULoop", 4096, NULL, configMAX_PRIORITIES - 1, nullptr, 1);
    }
    
    bool onboard_LED = 0;
    while(true){
        onboard_LED = !onboard_LED;
        gpio_set_level(GPIO_NUM_48, onboard_LED);  // heart beat LED  
        
        tm time;
        
        if (IO::Get()->rtc_handle->getTime(time) == ESP_OK) {
            char buf[32];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &time);
            printf("RTC Time: %s\n", buf);
        } else {
            printf("Failed to read RTC time\n");
        }
        // Logger::LogMessage_t test;
        // sprintf(test.label, "MAIN");
        // sprintf(test.message, "yert,1,2,3");
        // Logger::writeLine(test);


        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));
    }
}