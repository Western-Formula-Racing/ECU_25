#include "StateMachine.hpp"

using namespace StateMachine;
int64_t rtd_start_time;
float throttle;
float brake_pressure;
int rtd_button;
BMS::STATE pack_status;
nvs_handle_t nvs_storage_handle;
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

    else if (rtd_button && brake_pressure >= BRAKE_THRESHOLD)
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
    else if ( rtd_button && (current_time - rtd_start_time) < RTD_TIME )
    {
        IO::Get()->HSDWrite(ECU_48_HSD6, true);
        nextState = STARTUP_DELAY;
    }
    else if(rtd_button && (current_time - rtd_start_time) >= RTD_TIME )
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
    
    else if(pack_status==BMS::ACTIVE){
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
    
    setupAppsCalibration();

    for(;;){ 
        // anything that runs in all state's should go here
        //Sensors checked in all states:
        Sensors::Get()->poll_sensors();
        checkNewAppsCalibration();
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
        printf(">throttle:%.2f\n", throttle);
        printf(">butoon:%d\n", rtd_button);
        //lights
        IO::Get()->HSDWrite(ECU_48_HSD6, false);
        if(Pedals::Get()->getBrakePressure() >= BRAKE_LIGHT_THRESHOLD){
            HSD5_ID2012.set(true);
        }
        else{
            HSD5_ID2012.set(false);
        }
        printf(">IMD_light:%d\n", !IMDRelay_ID1056.get_bool());
        printf(">AMS_light:%d\n", !AMSRelay_ID1056.get_bool());
        IO::Get()->HSDWrite(ECU_41_HSD5, !IMDRelay_ID1056.get_bool());
        IO::Get()->HSDWrite(ECU_40_HSD4, !AMSRelay_ID1056.get_bool());
        IO::Get()->HSDWrite(ECU_37_HSD1, true);
        IO::Get()->HSDWrite(ECU_38_HSD2, true);
        IO::Get()->HSDWrite(ECU_39_HSD3, true);
        state = states[state]();
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}

void StateMachine::setupAppsCalibration()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    err = nvs_open("storage", NVS_READWRITE, &nvs_storage_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Done\n");
    }
    uint16_t app1_stored_min = 0;
    uint16_t app1_stored_max = 0;
    uint16_t app2_stored_min = 0;
    uint16_t app2_stored_max = 0;

    err = nvs_get_u16(nvs_storage_handle, "app1_stored_min", &app1_stored_min);
    switch (err) {
        case ESP_OK:
            printf("Done\n");
            printf("app1_stored_min = %" PRIu16 "\n", app1_stored_min);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    err = nvs_get_u16(nvs_storage_handle, "app1_stored_max", &app1_stored_max);
    switch (err) {
        case ESP_OK:
            printf("Done\n");
            printf("app1_stored_max = %" PRIu16 "\n", app1_stored_max);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    err = nvs_get_u16(nvs_storage_handle, "app2_stored_min", &app2_stored_min);
    switch (err) {
        case ESP_OK:
            printf("Done\n");
            printf("app2_stored_min = %" PRIu16 "\n", app2_stored_min);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    err = nvs_get_u16(nvs_storage_handle, "app2_stored_max", &app2_stored_max);
    switch (err) {
        case ESP_OK:
            printf("Done\n");
            printf("app2_stored_max = %" PRIu16 "\n", app2_stored_max);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    Pedals::Get()->updateAppsCalibration(static_cast<float>(app1_stored_min)/100.f,
                                        static_cast<float>(app1_stored_max)/100.f,
                                        static_cast<float>(app2_stored_min)/100.f,
                                        static_cast<float>(app2_stored_max)/100.f);
}


void StateMachine::checkNewAppsCalibration()
{
    float app1_min = apps1_min_ID2022.get_float();
    float app1_max = apps1_max_ID2022.get_float();
    float app2_min = apps2_min_ID2022.get_float();
    float app2_max = apps2_max_ID2022.get_float();
    if (app1_min != 0.0f && app1_max!=0.0f && app2_min != 0.0f && app2_max != 0.0f){
        //store values
        esp_err_t err = nvs_set_u16(nvs_storage_handle, "app1_stored_min", static_cast<uint16_t>(app1_min*100.0f));
        ESP_LOGW(TAG,"app1_min = %.2f", app1_min);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        err = nvs_set_u16(nvs_storage_handle, "app1_stored_max", static_cast<uint16_t>(app1_max*100.0f));
        ESP_LOGE(TAG,"app1_max = %.2f", app1_max);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        err = nvs_set_u16(nvs_storage_handle, "app2_stored_min", static_cast<uint16_t>(app2_min*100.0f));
        ESP_LOGW(TAG,"app2_min = %.2f", app1_min);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        err = nvs_set_u16(nvs_storage_handle, "app2_stored_max", static_cast<uint16_t>(app2_max*100.0f));
        printf("app2_max = %.2f", app2_max);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        Pedals::Get()->updateAppsCalibration(app1_min,app1_max,app2_min,app2_max);
    }
    
}