#include "CAN.h"
#include <stdio.h>
#include "esp_log.h"
static const char* TAG = "CAN"; //Used for ESP_LOGx commands. See ESP-IDF Documentation
static SemaphoreHandle_t rx_sem = xSemaphoreCreateBinary();


int give_zero(){
    printf("kill me please I can't keep doing this\n");
    return 0;
}




CAN::CAN(gpio_num_t CAN_TX_Pin, gpio_num_t CAN_RX_Pin, twai_mode_t twai_mode)
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_Pin, CAN_RX_Pin, twai_mode);
    g_config.rx_queue_len = 1000;
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
    twai_reconfigure_alerts_v2(twai_handle, TWAI_ALERT_RX_QUEUE_FULL, NULL);

}

void CAN::begin()
{
    xSemaphoreGive(rx_sem); // you have to give the semaphore on startup lol
    // Create the task
    xTaskCreatePinnedToCore(CAN::rx_task_wrapper, "CAN_rx", 4096, this, configMAX_PRIORITIES - 20, nullptr, 1);
}

void CAN::rx_task_wrapper(void *arg)
{
    CAN *instance = static_cast<CAN*>(arg);
    instance->rx_task();
}

void CAN::rx_task()
{
    
    while (true) {
        // Try to take the semaphore without blocking
        if (xSemaphoreTake(rx_sem, 0) == pdTRUE) {
            twai_message_t rx_msg;
            twai_read_alerts_v2(twai_handle, &twai_alerts, 0);
            if(twai_alerts & TWAI_ALERT_RX_QUEUE_FULL){
                    ESP_LOGW(TAG, "rx queue full: msg dropped");
                }
            while (twai_receive(&rx_msg, portMAX_DELAY) == ESP_OK) {
                // ESP_LOGI(TAG, "Rx'd ID %" PRIu32, rx_msg.identifier);
                // for (int i = 0; i < rx_msg.data_length_code; i++) {
                //     ESP_LOGI(TAG, "  %d", rx_msg.data[i]);
                // }
            }
            xSemaphoreGive(rx_sem);
        } else {
            // Could not get semaphore, yield
            taskYIELD();
        }
    }
}