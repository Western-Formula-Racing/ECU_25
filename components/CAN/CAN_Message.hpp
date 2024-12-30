#ifndef __CANMSG__
#define __CANMSG__
#include <stdio.h>
#include "esp_log.h"
#include <variant>
#include "CAN_Signal.hpp"
#include "etl/vector.h"
class CAN_Message
{
public:
    CAN_Message(uint16_t msg_id);
    etl::vector<CAN_Signal, 64> signals;
    uint16_t msg_id;
    
private:


};
#endif