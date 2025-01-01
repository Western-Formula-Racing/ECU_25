#ifndef __CAN_CONFIG__
#define __CAN_CONFIG__
#include "CAN_Signal.hpp"
#include "etl/map.h"
#include "etl/vector.h"

// Defining the CAN_IDs & Signals
#define INV_CommandMessage 192
inline CAN_Signal torqueRequest_Signal{true, 0, 16, FLOAT, 0.1f};
inline CAN_Signal directionCommand_Signal{true, 32, 1};
inline CAN_Signal inverterEnable_Signal{true, 40, 1};
inline CAN_Signal inverterDischarge_Signal{true, 41, 1};
inline CAN_Signal speedModeEnable_Signal{true, 42, 1};
inline CAN_Signal torqueLimit_Signal{true, 48, 16};

//define the CAN Map here
inline etl::map CAN_Map
{ 
    etl::pair{INV_CommandMessage, etl::vector<CAN_Signal*, 6>{ 
        &torqueRequest_Signal,
        &directionCommand_Signal,
        &inverterEnable_Signal,
        &inverterDischarge_Signal,
        &speedModeEnable_Signal,
        &torqueLimit_Signal
        }},

};




#endif 
