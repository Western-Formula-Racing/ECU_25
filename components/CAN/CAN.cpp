#include "CAN.h"
#include <stdio.h>
#include "esp_log.h"
#include "CAN_Config.hpp"
#include "can_helpers.hpp"
#include "Logger.h"

char log_string[256];
static const char *TAG = "CAN"; // Used for ESP_LOGx commands. See ESP-IDF Documentation
static SemaphoreHandle_t rx_sem = xSemaphoreCreateBinary();
static TimerHandle_t timerHandle;
static twai_message_t tx_message = {
    // Message type and format settings
    .extd = 0,         // Standard vs extended format
    .rtr = 0,          // Data vs RTR frame
    .ss = 0,           // Whether the message is single shot (i.e., does not repeat on error)
    .self = 0,         // Whether the message is a self reception request (loopback)
    .dlc_non_comp = 0, // DLC is less than 8
    // Message ID and payload
    .data_length_code = 8,
};
int give_zero()
{
    printf("kill me please I can't keep doing this\n");
    return 0;
}

CAN::CAN(gpio_num_t CAN_TX_Pin, gpio_num_t CAN_RX_Pin, twai_mode_t twai_mode)
{
    logging = false;
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_Pin, CAN_RX_Pin, twai_mode);
    g_config.rx_queue_len = 1000;
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    g_config.controller_id = 0;

    txCallBackCounter = 0;

    if (twai_driver_install_v2(&g_config, &t_config, &f_config, &twai_handle) == ESP_OK)
    {
        ESP_LOGI(TAG, "Driver installed\n");
    }
    else
    {
        ESP_LOGI(TAG, "Failed to install driver\n");
        return;
    }
    if (twai_start_v2(twai_handle) == ESP_OK)
    {
        ESP_LOGI(TAG, "Driver started\n");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to start driver\n");
        return;
    }
}

void CAN::begin()
{
    xSemaphoreGive(rx_sem); // you have to give the semaphore on startup lol
    // Create the task
    twai_reconfigure_alerts_v2(twai_handle, TWAI_ALERT_RX_QUEUE_FULL, NULL);
    timerHandle = xTimerCreate("canTx", pdMS_TO_TICKS(10), pdTRUE, (void *)this, tx_CallBack_wrapper);
    if (timerHandle == NULL)
    {
        ESP_LOGE(TAG, "failed to start CAN TxTimer\n");
    }
    else
    {
        if (xTimerStart(timerHandle, 0) != pdPASS)
        {
            ESP_LOGE(TAG, "failed to start CAN TxTimer\n");
        }
    }
    xTaskCreatePinnedToCore(CAN::rx_task_wrapper, "CAN_rx", 4096, this, configMAX_PRIORITIES - 20, nullptr, 1);
}

void CAN::rx_task_wrapper(void *arg)
{
    CAN *instance = static_cast<CAN *>(arg);
    instance->rx_task();
}

void CAN::tx_CallBack_wrapper(TimerHandle_t xTimer)
{
    CAN *instance = static_cast<CAN *>(static_cast<void *>(xTimer));
    instance->tx_CallBack();
}
/*
    One Decision I've made is that the actual CAN handler will not
    deal with type conversions and scaling. It's only pulling the raw 
    value from the bitfield and handling endian-ness (untested).
    Everything else is left up to the signal handler.


*/
void CAN::rx_task()
{

    while (true)
    {
        // Try to take the semaphore without blocking
        if (xSemaphoreTake(rx_sem, 0) == pdTRUE)
        {
            twai_message_t rx_msg;
            twai_read_alerts_v2(twai_handle, &twai_alerts, 0);
            if (twai_alerts & TWAI_ALERT_RX_QUEUE_FULL)
            {
                ESP_LOGW(TAG, "rx queue full: msg dropped");
            }
            while (twai_receive(&rx_msg, portMAX_DELAY) == ESP_OK)
            {
                if(logging){
                    sprintf(log_string, "%ld,", rx_msg.identifier);
                    for (int i = 0; i < rx_msg.data_length_code; i++)
                    {
                        char val[5];
                        sprintf(val, "%d,",rx_msg.data[i]);
                        strcat(log_string, val);
                    }
                    Logger::LogMessage_t log_message;
                    sprintf(log_message.label, "CAN");
                    sprintf(log_message.message, "%s", log_string);
                    Logger::writeLine(log_message);
                }
                if (CAN_Rx_IDs.find(rx_msg.identifier) != CAN_Rx_IDs.end())
                {
                    ESP_LOGI(TAG, "Rx'd ID %" PRIu32, rx_msg.identifier);
                    for (int i = 0; i < rx_msg.data_length_code; i++)
                    {
                        ESP_LOGI(TAG, "  %d", rx_msg.data[i]);
                    }
                    if (CAN_Map.find(rx_msg.identifier) != CAN_Map.end())
                    {
                        for (const auto &signal : CAN_Map[rx_msg.identifier])
                        {
                            signal->set_raw(can_getSignal<uint64_t>(rx_msg.data, signal->startBit, signal->length, signal->isIntel));
                        }
                    }
                }
            }
            xSemaphoreGive(rx_sem);
        }
        else
        {
            // Could not get semaphore, yield
            taskYIELD();
        }
    }
}

void CAN::tx_CallBack()
{
    txCallBackCounter = (txCallBackCounter < 1000) ? txCallBackCounter + 1 : 0;

    // I don't believe in 1ms messages in 2025. nothing's that important

    // 10ms messages
    for (const auto &identifier : CAN_Tx_10ms_IDs)
    {
        for (const auto &signal : CAN_Map[identifier])
        {
            tx_message.identifier = identifier;
            tx_message.data_length_code = 8;
            can_setSignal<uint64_t>(tx_message.data, signal->get_raw(), signal->startBit, signal->length, signal->isIntel);
        }
        if (twai_transmit(&tx_message, pdMS_TO_TICKS(1000)) != ESP_OK)
        {
            ESP_LOGE(TAG, "failed to tx message\n");
        }
    }
    // 100ms messages
    if (txCallBackCounter % 10 == 0)
    {
        for (const auto &identifier : CAN_Tx_100ms_IDs)
        {
            for (const auto &signal : CAN_Map[identifier])
            {
                tx_message.identifier = identifier;
                tx_message.data_length_code = 8;
                can_setSignal<uint64_t>(tx_message.data, signal->get_raw(), signal->startBit, signal->length, signal->isIntel);
            }
            if (twai_transmit(&tx_message, pdMS_TO_TICKS(1000)) != ESP_OK)
            {
                ESP_LOGE(TAG, "failed to tx message\n");
            }
        }
    }
    // 1000ms messages
    if (txCallBackCounter % 100 == 0)
    {
        for (const auto &identifier : CAN_Tx_1000ms_IDs)
        {
            for (const auto &signal : CAN_Map[identifier])
            {
                tx_message.identifier = identifier;
                tx_message.data_length_code = 8;
                can_setSignal<uint64_t>(tx_message.data, signal->get_raw(), signal->startBit, signal->length, signal->isIntel);
            }
            if (twai_transmit(&tx_message, pdMS_TO_TICKS(1000)) != ESP_OK)
            {
                ESP_LOGE(TAG, "failed to tx message\n");
            }
        }
    }
}