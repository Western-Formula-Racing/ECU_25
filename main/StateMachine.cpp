#include "StateMachine.hpp"

using namespace StateMachine;

State StateMachine::handle_start()
{
    State nextState = START;

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
        printf("state machine running!\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }

}