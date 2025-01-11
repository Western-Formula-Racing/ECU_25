#ifndef __CAN_CONFIG__
#define __CAN_CONFIG__
#include "CAN_Signal.hpp"
#include "etl/map.h"
#include "etl/vector.h"
#include "etl/set.h"

// Defining the CAN_IDs & Signals
#define INV_M192_Command_Message 192
inline CAN_Signal torqueRequest_Signal{true, 0, 16, FLOAT, 0.1f};
inline CAN_Signal directionCommand_Signal{true, 32, 1};
inline CAN_Signal inverterEnable_Signal{true, 40, 1};
inline CAN_Signal inverterDischarge_Signal{true, 41, 1};
inline CAN_Signal speedModeEnable_Signal{true, 42, 1};
inline CAN_Signal torqueLimit_Signal{true, 48, 16};

#define INV_M176_Fast_Info 176
inline CAN_Signal INV_Fast_DC_Bus_Voltage{true, 48, 16, FLOAT, 0.1f};
inline CAN_Signal INV_Fast_Motor_Speed{true, 32, 16};
inline CAN_Signal INV_Fast_Torque_Command{true, 0, 16, FLOAT, 0.1f};
inline CAN_Signal INV_Fast_Torque_Feedback{true, 16, 16, FLOAT, 0.1f};

//define the CAN Map here
//all entries in the map must have the exact same types which means all the vectors have to be the same size
inline etl::map CAN_Map
{ 
    etl::pair{INV_M192_Command_Message, etl::vector<CAN_Signal*, 8>{ 
        &torqueRequest_Signal,
        &directionCommand_Signal,
        &inverterEnable_Signal,
        &inverterDischarge_Signal,
        &speedModeEnable_Signal,
        &torqueLimit_Signal
        }},
    etl::pair{INV_M176_Fast_Info, etl::vector<CAN_Signal*, 8>{ 
        &INV_Fast_DC_Bus_Voltage,
        &INV_Fast_Motor_Speed,
        &INV_Fast_Torque_Command,
        &INV_Fast_Torque_Feedback,
        }},
};

inline etl::set CAN_Rx_IDs = {176, 192};
inline etl::set CAN_Tx_10ms_IDs = {192};
// inline etl::set CAN_Tx_100ms_IDs = {};
// inline etl::set CAN_Tx_1000ms_IDs = {};
//they don't like to be empty 

#endif 
