#include "StateMachine.hpp"

using namespace StateMachine;

State StateMachine::handle_start()
{
    State nextState = START;
    int rtd_button = IO::Get()->digitalRead(ECU_TEST);
    // int rtd_button = 1;
    PACK_STATE pack_status = (PACK_STATE)PackStatus_ID1056.get_int();
    printf(">status_raw:%lld\n", PackStatus_ID1056.get_raw());
    printf(">RTD_BUTTON:%d\n>pack_status:%d\n", rtd_button, pack_status);

    if (pack_status == PACK_ACTIVE && rtd_button)
    {
        nextState = STARTUP_DELAY;
    }


    return nextState;
}

State StateMachine::handle_precharge_enable()
{
    State nextState = PRECHARGE_ERROR;

    return nextState;
}

State StateMachine::handle_precharge_ok()
{
    State nextState = PRECHARGE_OK;

    return nextState;
}

State StateMachine::handle_startup_delay()
{
    State nextState = STARTUP_DELAY;

    return nextState;
}

State StateMachine::handle_drive()
{
    State nextState = START;

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
        state = states[state]();
        printf(">state:%d\n", state);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}