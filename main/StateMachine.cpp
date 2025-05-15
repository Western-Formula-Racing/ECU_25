#include "StateMachine.hpp"

using namespace StateMachine;
int64_t rtd_start_time;
float throttle;
int rtd_button;
BMS::STATE pack_status;

static const char* TAG = "State Machine"; //Used for ESP_LOGx commands. See ESP-IDF Documentation

State StateMachine::handle_start()
{
    State nextState = START;
    Inverter::Get()->Disable();
    
    if (pack_status == BMS::FAULT)
    {
        ESP_LOGE(TAG, "BMS FAULT Detected");
        nextState = DEVICE_FAULT;
    }

    else if (pack_status == BMS::PRECHARGE)
    {
        nextState = PRECHARGE_ENABLE;
    }

    else if (pack_status == BMS::ACTIVE && rtd_button)
    {
        rtd_start_time = esp_timer_get_time()/1000;
        nextState = STARTUP_DELAY;
    }


    return nextState;
}

State StateMachine::handle_precharge_enable()
{
    State nextState = PRECHARGE_ENABLE;
    Inverter::Get()->Disable();
    
    if (pack_status == BMS::FAULT)
    {
        ESP_LOGE(TAG, "BMS Encountered an error during precharge");
        nextState = PRECHARGE_ERROR;
    }
    
    else if (pack_status == BMS::ACTIVE)
    {
        nextState = PRECHARGE_OK;
    }
    return nextState;
}

State StateMachine::handle_precharge_ok()
{
    State nextState = PRECHARGE_OK;
    Inverter::Get()->Disable();

    if (pack_status == BMS::ACTIVE && rtd_button)
    {
        rtd_start_time = esp_timer_get_time()/1000;
        nextState = STARTUP_DELAY;
    }

    else if (pack_status == BMS::FAULT)
    {
        return DEVICE_FAULT;
    }

    return nextState;
}

State StateMachine::handle_startup_delay()
{
    State nextState = START;
    Inverter::Get()->Disable();
    
   
    int64_t current_time = esp_timer_get_time()/1000;
    if (pack_status == BMS::FAULT){
        ESP_LOGE(TAG, "BMS FAULT Detected during startup delay");
        nextState = DEVICE_FAULT;
    }
    else if (pack_status == BMS::ACTIVE && rtd_button && (current_time - rtd_start_time) < RTD_TIME )
    {
        // sound buzzer
        nextState = STARTUP_DELAY;
    }
    else if(pack_status == BMS::ACTIVE && rtd_button && (current_time - rtd_start_time) >= RTD_TIME )
    {
        nextState = DRIVE;
    }
    return nextState;
}

State StateMachine::handle_drive()
{
    State nextState = START;
    Inverter::Get()->Enable();
    if (pack_status == BMS::FAULT){
        ESP_LOGE(TAG, "BMS FAULT Detected during drive");
        nextState = DEVICE_FAULT;
    }

    else if(pack_status == BMS::ACTIVE){
        nextState = DRIVE;
        Inverter::Get()->SetTorqueRequest(throttle);
    }

    return nextState;
}
State StateMachine::handle_precharge_error()
{
    Inverter::Get()->Disable();
    return PRECHARGE_ERROR;
}
State StateMachine::handle_device_fault()
{
    Inverter::Get()->Disable();
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
        // anything that runs in all state's should go here
        //Sensors checked in all states:
        Sensors::Get()->poll_sensors();

        pack_status = BMS::Get()->getPackState();
        rtd_button = IO::Get()->digitalRead(ECU_TEST);
        throttle = Pedals::Get()->getThrottle();
        
        State_ID2002.set(state);

        printf(">packStatus:%d\n", pack_status);
        printf(">state:%d\n", state);

        state = states[state]();
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}