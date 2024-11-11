#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "CAN.h"
#include "Sensors.h"
#include "IO.h"
#include "Devices.h"

// must extern C since this gets called by a C file for freeRTOS
extern "C" void app_main(void)
{
    int x = 0;
    while(true){
        printf("x: %d\n", x);
        x++;
        vTaskDelay(10);
    }
    
}