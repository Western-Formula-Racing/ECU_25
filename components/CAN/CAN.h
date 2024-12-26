#ifndef __CAN__
#define __CAN__

#include <map>
#include "driver/twai.h"
#include "etl/vector.h"

class CAN
{

public:
    CAN(gpio_num_t CAN_TX_Pin, gpio_num_t CAN_RX_Pin, twai_mode_t twai_mode);
    
    
private:
    twai_handle_t twai_handle;
    twai_general_config_t g_config;
    twai_timing_config_t t_config;
    twai_filter_config_t f_config;

    void rx_task();
    void tx_task();

    



};



int give_zero();

#endif