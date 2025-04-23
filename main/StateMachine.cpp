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
    IO::Get()->HSDWrite(ECU_39_HSD3, false);
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
    IO::Get()->HSDWrite(ECU_39_HSD3, false);
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
    IO::Get()->HSDWrite(ECU_39_HSD3, false);
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
    IO::Get()->HSDWrite(ECU_39_HSD3, false);
   
    int64_t current_time = esp_timer_get_time()/1000;
    if (pack_status == BMS::FAULT){
        ESP_LOGE(TAG, "BMS FAULT Detected during startup delay");
        nextState = DEVICE_FAULT;
    }
    else if (pack_status == BMS::ACTIVE && rtd_button && (current_time - rtd_start_time) < RTD_TIME )
    {
        IO::Get()->HSDWrite(ECU_48_HSD6, true);
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
    IO::Get()->HSDWrite(ECU_39_HSD3, true);
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
    IO::Get()->HSDWrite(ECU_39_HSD3, false);
    return PRECHARGE_ERROR;
}
State StateMachine::handle_device_fault()
{
    Inverter::Get()->Disable();
    IO::Get()->HSDWrite(ECU_39_HSD3, false);
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
        //this should get bundled into the sensor class later
        A1_ID2014.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(0)));
        A2_ID2014.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(1)));
        A3_ID2014.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(2)));
        A4_ID2014.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(3)));
        A5_ID2015.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(4)));
        A6_ID2015.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(5)));
        A7_ID2015.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(6)));
        A8_ID2015.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(7)));
        A9_ID2016.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(8)));
        A10_ID2016.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(9)));
        A11_ID2016.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(10)));
        A12_ID2016.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(11)));
        A13_ID2017.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(12)));

        pack_status = BMS::Get()->getPackState();
        rtd_button = !IO::Get()->digitalRead(ECU_10_IO1);
        throttle = Pedals::Get()->getThrottle();
        
        State_ID2002.set(state);

        printf(">packStatus:%d\n", pack_status);
        printf(">state:%d\n", state);
        //lights
        IO::Get()->HSDWrite(ECU_48_HSD6, false);
        if(Pedals::Get()->getBrakePressure() >= BRAKE_THRESHOLD){
            HSD5_ID2012.set(true);
        }
        else{
            HSD5_ID2012.set(false);
        }
        IO::Get()->HSDWrite(ECU_41_HSD5, !IMDRelay_ID1056.get_bool());
        IO::Get()->HSDWrite(ECU_48_HSD6, !AMSRelay_ID1056.get_bool());

        state = states[state]();
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}