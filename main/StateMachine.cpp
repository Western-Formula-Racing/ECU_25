#include "StateMachine.hpp"

using namespace StateMachine;
int64_t rtd_start_time;
float throttle;
int rtd_button;

State StateMachine::handle_start()
{
    State nextState = START;
    VCU_INV_Torque_Command_ID192.set(0);
    
    PACK_STATE pack_status = (PACK_STATE)PackStatus_ID1056.get_int();
    printf(">packStatus:%d\n", pack_status);
    if (pack_status == PACK_ACTIVE && rtd_button)
    {
        rtd_start_time = esp_timer_get_time()/1000;
        nextState = STARTUP_DELAY;
    }


    return nextState;
}

State StateMachine::handle_precharge_enable()
{
    State nextState = PRECHARGE_ERROR;
    VCU_INV_Torque_Command_ID192.set(0);
    return nextState;
}

State StateMachine::handle_precharge_ok()
{
    State nextState = PRECHARGE_OK;
    VCU_INV_Torque_Command_ID192.set(0);
    return nextState;
}

State StateMachine::handle_startup_delay()
{
    State nextState = START;
    VCU_INV_Torque_Command_ID192.set(0);
    rtd_button = IO::Get()->digitalRead(ECU_TEST);
    // int rtd_button = 1;
    PACK_STATE pack_status = (PACK_STATE)PackStatus_ID1056.get_int();
    int64_t current_time = esp_timer_get_time()/1000;
    if (pack_status == PACK_ACTIVE && rtd_button && (current_time - rtd_start_time) < RTD_TIME )
    {
        // sound buzzer
        nextState = STARTUP_DELAY;
    }
    else if(pack_status == PACK_ACTIVE && rtd_button && (current_time - rtd_start_time) >= RTD_TIME )
    {
        nextState = DRIVE;
    }
    return nextState;
}

State StateMachine::handle_drive()
{
    State nextState = START;
    PACK_STATE pack_status = (PACK_STATE)PackStatus_ID1056.get_int();
    if(pack_status == PACK_ACTIVE){
        nextState = DRIVE;
        VCU_INV_Torque_Command_ID192.set(50*Pedals::Get()->getThrottle());
    }

    return nextState;
}
State StateMachine::handle_precharge_error()
{
    return PRECHARGE_ERROR;
}
State StateMachine::handle_device_fault()
{
    return DEVICE_FAULT;
}

void StateMachine::StateMachineLoop(void *)
{
    etl::array<state_function_t, 8> states = {
        handle_start,
        handle_precharge_enable,
        handle_precharge_ok,
        handle_startup_delay,
        handle_drive,
        handle_precharge_error,
        handle_device_fault,
    };
    State state = START;
    
    for(;;){ 
        //Sensors checked in all states:
        rtd_button = IO::Get()->digitalRead(ECU_TEST);
        state = states[state]();
        printf(">state:%d\n", state);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}