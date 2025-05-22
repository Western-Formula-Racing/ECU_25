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

const char* get_twai_error_state_text(twai_status_info_t* status)
{
    if (status->state == TWAI_STATE_STOPPED) return "Stopped";
    else if (status->state == TWAI_STATE_RUNNING) {
        if (status->tx_error_counter >= 128 || status->rx_error_counter >= 128) {
            return "Bus-Off or Error-Passive";
        } else if (status->tx_error_counter > 0 || status->rx_error_counter > 0) {
            return "Error-Active (with some errors)";
        } else {
            return "Error-Active (no errors)";
        }
    } else {
        return "Unknown";
    }
}
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
        can0.logging = true;
        // xTaskCreatePinnedToCore(RearECU::rearECU_Task, "RearECULoop", 4096, NULL, configMAX_PRIORITIES - 1, nullptr, 1);
    }
    
    bool onboard_LED = 0;
    while(true){
        onboard_LED = !onboard_LED;
        gpio_set_level(GPIO_NUM_48, onboard_LED);  // heart beat LED  
        
        twai_status_info_t status;
        esp_err_t ret = twai_get_status_info(&status);
        if (ret == ESP_OK) {
            printf("TWAI state: %s | TX errors: %ld | RX errors: %ld | Bus Errors: %ld\n",
                   get_twai_error_state_text(&status),
                   status.tx_error_counter,
                   status.rx_error_counter,
                   status.bus_error_count);
        } else {
            printf("Failed to get TWAI status: %s\n", esp_err_to_name(ret));
        }


        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));
    }
}