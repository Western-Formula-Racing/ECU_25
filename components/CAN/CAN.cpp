#include "CAN.h"
#include <stdio.h>
#include "esp_log.h"
static const char* TAG = "CAN"; //Used for ESP_LOGx commands. See ESP-IDF Documentation
int give_zero(){
    printf("kill me please I can't keep doing this\n");
    return 0;
}