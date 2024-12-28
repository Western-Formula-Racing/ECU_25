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

static const char* TAG = "Main"; //Used for ESP_LOGx commands. See ESP-IDF Documentation
CAN can0{GPIO_NUM_47, GPIO_NUM_48, TWAI_MODE_NORMAL};


// must extern C since this gets called by a C file for freeRTOS
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Main Has begun");
    while(true){
        printf("test\n");
        vTaskDelay(10);
    }
    
}