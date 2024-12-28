#ifndef __CAN__
#define __CAN__

#include <map>
#include "driver/twai.h"
#include "etl/queue.h"
#include <stdio.h>
#include "esp_log.h"

class CAN
{
public:
    CAN(gpio_num_t CAN_TX_Pin, gpio_num_t CAN_RX_Pin, twai_mode_t twai_mode);
    static void rx_task_wrapper(void *arg); // Wrapper function
    void rx_task(); // Non-static member function

private:
    twai_handle_t twai_handle;
    twai_general_config_t g_config;
    twai_timing_config_t t_config;
    twai_filter_config_t f_config;
    static SemaphoreHandle_t rx_sem;

    void tx_task();
};

int give_zero();

#endif