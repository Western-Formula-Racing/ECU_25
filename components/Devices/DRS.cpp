#include "DRS.hpp"
#include "CAN_Config.hpp"
#include "esp_log.h"

static const char* TAG = "DRS";

static void send_servo(bool open) {
    uint16_t position = open ? DRS::POSITION_OPEN : DRS::POSITION_CLOSED;
    uint8_t lo = position & 0xFF;
    uint8_t hi = (position >> 8) & 0xFF;

    twai_message_t msg = {};
    msg.identifier       = 0;     
    msg.extd             = 0;
    msg.data_length_code = 5;
    msg.data[0] = 'w';             
    msg.data[1] = 0;              
    msg.data[2] = 0x1E;            
    msg.data[3] = lo;
    msg.data[4] = hi;

    twai_transmit(&msg, pdMS_TO_TICKS(10));
}

void DRS::init() {
    send_servo(false);  // start closed
    ESP_LOGI(TAG, "DRS initialised — wing CLOSED");
}

void DRS::update() {
    bool open = DRS_Request_ID103.get_bool();
    send_servo(open);
    ESP_LOGD(TAG, ">DRS_open:%d", open);
}