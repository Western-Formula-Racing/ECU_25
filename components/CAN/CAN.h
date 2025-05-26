#ifndef __CAN__
#define __CAN__

#include <map>
#include "driver/twai.h"
#include "etl/vector.h"
#include <stdio.h>
#include "esp_log.h"
#include <variant>

class CAN
{
public:
    CAN(gpio_num_t CAN_TX_Pin, gpio_num_t CAN_RX_Pin, twai_mode_t twai_mode);
    void begin();
    bool logging;
    int restart_counter;
    void restart(gpio_num_t CAN_TX_Pin, gpio_num_t CAN_RX_Pin, twai_mode_t twai_mode);
private:
    twai_handle_t twai_handle;
    twai_general_config_t g_config;
    twai_timing_config_t t_config;
    twai_filter_config_t f_config;
    uint32_t twai_alerts;
    uint16_t txCallBackCounter; 

    static void rx_task_wrapper(void *arg); // Wrapper function
    static void tx_CallBack_wrapper(TimerHandle_t xTimer); // Wrapper function
    void tx_CallBack(); // Non-static member function
    void rx_task(); // Non-static member function
    
};

int give_zero();

#endif