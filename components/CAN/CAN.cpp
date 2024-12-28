#include "CAN.h"
#include <stdio.h>
#include "esp_log.h"
static const char* TAG = "CAN"; //Used for ESP_LOGx commands. See ESP-IDF Documentation
int give_zero(){
    printf("kill me please I can't keep doing this\n");
    return 0;
}




CAN::CAN(gpio_num_t CAN_TX_Pin, gpio_num_t CAN_RX_Pin, twai_mode_t twai_mode)
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_Pin, CAN_RX_Pin, twai_mode);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    g_config.controller_id = 0;
    if (twai_driver_install_v2(&g_config, &t_config, &f_config, &twai_handle) == ESP_OK) {
        ESP_LOGI(TAG,"Driver installed\n");
    } else {
        ESP_LOGI(TAG,"Failed to install driver\n");
        return;
    }
    if (twai_start_v2(twai_handle) == ESP_OK) {
        ESP_LOGI(TAG,"Driver started\n");
    } else {
        ESP_LOGI(TAG,"Failed to start driver\n");
        return;
    }
    // Create the task
    xTaskCreatePinnedToCore(CAN::rx_task_wrapper, "CAN_rx", 4096, this, configMAX_PRIORITIES - 1, nullptr, tskNO_AFFINITY);
}

void CAN::rx_task_wrapper(void *arg)
{
    CAN *instance = static_cast<CAN*>(arg);
    instance->rx_task();
}

void CAN::rx_task()
{
    // xSemaphoreTake(rx_sem, portMAX_DELAY);

    while (true) {
        twai_message_t rx_msg;
        twai_receive(&rx_msg, portMAX_DELAY);
        ESP_LOGI(TAG, "Received message with ID %" PRIu32, rx_msg.identifier);
        for (int i = 0; i < rx_msg.data_length_code; i++) {
             ESP_LOGI(TAG, "  %d" , rx_msg.data[i]);
        }
    }
}