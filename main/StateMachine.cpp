#include "StateMachine.hpp"

using namespace StateMachine;
int64_t rtd_start_time;
float throttle;
float brake_pressure;
int rtd_button;
uint64_t last_right_tick;
uint64_t last_left_tick;
uint64_t last_tick_time;
float right_rpm;
float left_rpm;
BMS::STATE pack_status;
nvs_handle_t nvs_storage_handle;
State state = START; 
static const char *TAG = "State Machine"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

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

    else if (pack_status == BMS::PRECHARGING)
    {
        nextState = PRECHARGE_ENABLE;
    }

    else if (pack_status == BMS::ACTIVE)
    {
        nextState = PRECHARGE_OK;
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
    if (pack_status == BMS::ACTIVE && rtd_button && (brake_pressure >= BRAKE_RTD_THRESHOLD))
    {
        rtd_start_time = esp_timer_get_time() / 1000;
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

    int64_t current_time = esp_timer_get_time() / 1000;
    if (pack_status == BMS::FAULT)
    {
        ESP_LOGE(TAG, "BMS FAULT Detected during startup delay");
        nextState = DEVICE_FAULT;
    }
    else if (rtd_button && (current_time - rtd_start_time) < RTD_TIME)
    {
        IO::Get()->HSDWrite(ECU_48_HSD6, true);
        nextState = STARTUP_DELAY;
    }
    else if (rtd_button && (current_time - rtd_start_time) >= RTD_TIME)
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
    if (pack_status == BMS::FAULT)
    {
        ESP_LOGE(TAG, "BMS FAULT Detected during drive");
        nextState = DEVICE_FAULT;
    }

    else if (pack_status == BMS::ACTIVE)
    {
        nextState = DRIVE;
        if(throttle>=0.05){
            Inverter::Get()->SetTorqueRequest(throttle-0.05);
        }
        else{
            Inverter::Get()->SetTorqueRequest(0);
        }
        
    }
    // if (MinCellVoltage_ID1057.get_float() <= 2.1f){
    //     nextState = START;
    // }
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
    state = START;

    setupAppsCalibration();
    int64_t startup_time = esp_timer_get_time() / 1000;
    //tssi enable
    HSD3_ID2012.set(true);
    HSD4_ID2012.set(true);
    bool tssi_latch = false;
    for (;;)
    {
        // anything that runs in all state's should go here
        // Sensors checked in all states:
        Sensors::Get()->poll_sensors();
        checkNewAppsCalibration();

        Accel_X_ID2024.set(IO::Get()->getAccelX());
        Accel_Y_ID2024.set(IO::Get()->getAccelY());
        Accel_Z_ID2024.set(IO::Get()->getAccelZ());
        Gyro_X_ID2025.set(IO::Get()->getGyroX());
        Gyro_Y_ID2025.set(IO::Get()->getGyroY());
        Gyro_Z_ID2025.set(IO::Get()->getGyroZ());
        pack_status = BMS::Get()->getPackState();
        rtd_button = !IO::Get()->digitalRead(ECU_10_IO1);
        throttle = Pedals::Get()->getThrottle();
        brake_pressure = Pedals::Get()->getBrakePressure();
        State_ID2002.set(state);
        // calculate wheel RPM
        // current_time = esp_timer_get_time();
        // right_rpm = (IO::right_wheel_tick - last_right_tick);
        // left_rpm = (IO::left_wheel_tick - last_left_tick);
        // right_rpm = (IO::right_wheel_tick - last_right_tick)/(4*6*(100000000000)*(current_time - last_tick_time));
        // left_rpm = (IO::left_wheel_tick - last_left_tick)/(4*6*(100000000000)*(current_time - last_tick_time));
        // printf(">deltaT:%lld\n",(4*6*(100000000000)*(current_time - last_tick_time)));
        // last_left_tick = IO::left_wheel_tick;
        // last_right_tick = IO::right_wheel_tick;
        // last_tick_time = current_time;

        printf(">packStatus:%d\n", pack_status);
        printf(">state:%d\n", state);
        printf(">throttle:%.2f\n", throttle);
        printf(">button:%d\n", rtd_button);
        printf(">brake:%.2f\n", brake_pressure);
        printf(">accel_x:%.2f\n", IO::Get()->getAccelX());
        printf(">accel_y:%.2f\n", IO::Get()->getAccelY());
        printf(">accel_z:%.2f\n", IO::Get()->getAccelZ());
        printf(">gyro_x:%.2f\n", IO::Get()->getGyroX());
        printf(">gyro_y:%.2f\n", IO::Get()->getGyroY());
        printf(">gyro_z:%.2f\n", IO::Get()->getGyroZ());
        printf(">rightWheel_tick:%lld\n", IO::right_wheel_tick);
        printf(">leftWheel_tick:%lld\n", IO::left_wheel_tick);
        printf(">minCellVoltage:%.2f\n", MinCellVoltage_ID1057.get_float());
        printf(">maxCellVoltage:%.2f\n", MaxCellVoltage_ID1057.get_float());

        Front_Left_Ticker_ID2028.set(IO::Get()->right_wheel_tick);
        Front_Right_Ticker_ID2029.set(IO::Get()->left_wheel_tick);

        Throttle_ID2002.set(throttle);
        Brake_Percent_ID2002.set(brake_pressure / BRAKES_MAX);

        // VCU_PEDAL_INFO (ID 2000)
        sensorPlausibility_ID2000.set(Pedals::Get()->is_faulted());
        implausibilityCounter_ID2000.set(Pedals::Get()->get_implausibility_counter());
        
        // lights
        IO::Get()->HSDWrite(ECU_48_HSD6, false);
        HSD4_ID2012.set(true);
        if (Pedals::Get()->getBrakePressure() >= BRAKE_LIGHT_THRESHOLD)
        {
            HSD5_ID2012.set(true);
        }
        else
        {
            HSD5_ID2012.set(false);
        }
        printf(">IMD_light:%d\n", !IMDRelay_ID1056.get_bool());
        printf(">AMS_light:%d\n", !AMSRelay_ID1056.get_bool());
        printf(">tssi_latch:%d\n", tssi_latch);
        
        
        if(pack_status == BMS::ACTIVE or pack_status == BMS::PRECHARGE_START or pack_status == BMS::PRECHARGING){
            IO::Get()->HSDWrite(ECU_38_HSD2, true);
        }
        else{
            IO::Get()->HSDWrite(ECU_38_HSD2, false);
            // HSD3_ID2012.set(false);
        }
        
        if(((esp_timer_get_time() / 1000) - startup_time) >= 3000){
            if (!IMDRelay_ID1056.get_bool()){
                IO::Get()->HSDWrite(ECU_41_HSD5, true);
             }
             IO::Get()->HSDWrite(ECU_40_HSD4, !AMSRelay_ID1056.get_bool());
             if(!IMDRelay_ID1056.get_bool()){
                //flash TSSI red >: (
                tssi_latch = true;
             }
             if(!AMSRelay_ID1056.get_bool()){
                tssi_latch = true;
             }
        }
        if(!tssi_latch){
            HSD3_ID2012.set(true);
        }
        else{
            HSD3_ID2012.set(false);
        }


        
        state = states[state]();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void StateMachine::setupAppsCalibration()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open("storage", NVS_READWRITE, &nvs_storage_handle);
    if (err != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        printf("Done\n");
    }
    uint16_t app1_stored_min = 0;
    uint16_t app1_stored_max = 0;
    uint16_t app2_stored_min = 0;
    uint16_t app2_stored_max = 0;

    err = nvs_get_u16(nvs_storage_handle, "app1_stored_min", &app1_stored_min);
    switch (err)
    {
    case ESP_OK:
        printf("Done\n");
        printf("app1_stored_min = %" PRIu16 "\n", app1_stored_min);
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        break;
    default:
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    err = nvs_get_u16(nvs_storage_handle, "app1_stored_max", &app1_stored_max);
    switch (err)
    {
    case ESP_OK:
        printf("Done\n");
        printf("app1_stored_max = %" PRIu16 "\n", app1_stored_max);
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        break;
    default:
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    err = nvs_get_u16(nvs_storage_handle, "app2_stored_min", &app2_stored_min);
    switch (err)
    {
    case ESP_OK:
        printf("Done\n");
        printf("app2_stored_min = %" PRIu16 "\n", app2_stored_min);
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        break;
    default:
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    err = nvs_get_u16(nvs_storage_handle, "app2_stored_max", &app2_stored_max);
    switch (err)
    {
    case ESP_OK:
        printf("Done\n");
        printf("app2_stored_max = %" PRIu16 "\n", app2_stored_max);
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        break;
    default:
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    Pedals::Get()->updateAppsCalibration(static_cast<float>(app1_stored_min) / 100.f,
                                         static_cast<float>(app1_stored_max) / 100.f,
                                         static_cast<float>(app2_stored_min) / 100.f,
                                         static_cast<float>(app2_stored_max) / 100.f);
}

void StateMachine::checkNewAppsCalibration()

{

    // Safety Guard: Only allow calibration when the car is in the START state

    if (state != START)

    {

        // If we were sampling and the car suddenly left the START state, abort

        if (calib_state != CALIB_IDLE)

        {

            ESP_LOGW(TAG, "Calibration ABORTED: State changed from START to %d", state);

            calib_state = CALIB_IDLE;

            calib_target = TARGET_NONE;

        }

        return;

    }





    // Manual Set (ID 2022) - Keep this for manual overrides if needed



    float app1_min = apps1_min_ID2022.get_float();

    float app1_max = apps1_max_ID2022.get_float();

    float app2_min = apps2_min_ID2022.get_float();

    float app2_max = apps2_max_ID2022.get_float();

        if (app1_min != 0.0f && app1_max != 0.0f && app2_min != 0.0f && app2_max != 0.0f)

        {

            nvs_set_u16(nvs_storage_handle, "app1_stored_min", static_cast<uint16_t>(app1_min * 100.0f));

            nvs_set_u16(nvs_storage_handle, "app1_stored_max", static_cast<uint16_t>(app1_max * 100.0f));

            nvs_set_u16(nvs_storage_handle, "app2_stored_min", static_cast<uint16_t>(app2_min * 100.0f));

            nvs_set_u16(nvs_storage_handle, "app2_stored_max", static_cast<uint16_t>(app2_max * 100.0f));

            Pedals::Get()->updateAppsCalibration(app1_min, app1_max, app2_min, app2_max);

    

        

        // Reset manual inputs to 0 to prevent re-triggering

        // Note: This relies on the sender stopping the message, as we can't clear RX signals easily.

    }



    // State Machine for Auto-Calibration (ID 2023)

    int64_t current_time = esp_timer_get_time() / 1000;



    switch (calib_state)

    {

    case CALIB_IDLE:

        if (set_min_ID2023.get_bool())

        {

            ESP_LOGI(TAG, "Calibration: MIN requested. Waiting 1s...");

            calib_target = TARGET_MIN;

            calib_state = CALIB_WAIT;

            calib_timer = current_time;

        }

        else if (set_max_ID2023.get_bool())

        {

            ESP_LOGI(TAG, "Calibration: MAX requested. Waiting 1s...");

            calib_target = TARGET_MAX;

            calib_state = CALIB_WAIT;

            calib_timer = current_time;

        }

        break;



    case CALIB_WAIT:

        if ((current_time - calib_timer) >= 1000)

        {

            ESP_LOGI(TAG, "Calibration: Starting 3s sampling...");

            calib_state = CALIB_SAMPLING;

            calib_timer = current_time;

            calib_samples_apps1.clear();

            calib_samples_apps2.clear();

        }

        break;



    case CALIB_SAMPLING:

        // Collect samples

        calib_samples_apps1.push_back(Pedals::Get()->get_apps1_voltage());

        calib_samples_apps2.push_back(Pedals::Get()->get_apps2_voltage());



        // Check if 3s has passed or vector full

        if ((current_time - calib_timer) >= 3000 || calib_samples_apps1.full())

        {

            ESP_LOGI(TAG, "Calibration: Sampling complete. Processing...");

            

            if (calib_samples_apps1.empty()) {

                calib_state = CALIB_IDLE;

                break;

            }



            // Sort to find median

            std::sort(calib_samples_apps1.begin(), calib_samples_apps1.end());

            std::sort(calib_samples_apps2.begin(), calib_samples_apps2.end());



            // Calculate stats

            float min1 = calib_samples_apps1.front();

            float max1 = calib_samples_apps1.back();

            float median1 = calib_samples_apps1[calib_samples_apps1.size() / 2];



            float min2 = calib_samples_apps2.front();

            float max2 = calib_samples_apps2.back();

            float median2 = calib_samples_apps2[calib_samples_apps2.size() / 2];



            ESP_LOGI(TAG, "Stats A9: Min=%.2f, Max=%.2f, Median=%.2f", min1, max1, median1);

            ESP_LOGI(TAG, "Stats A14: Min=%.2f, Max=%.2f, Median=%.2f", min2, max2, median2);



            // Apply Calibration

            if (calib_target == TARGET_MIN)

            {

                // Safety check: Don't calibrate if voltage is essentially 0 (open circuit)

                if (median1 < 0.5f || median2 < 0.5f) { // Assuming APPS2 isn't inverted to 0V at idle, check schematic if needed

                     // Actually APPS2 is inverted (High at 0%). 

                     // Wait, at 0% Throttle: APPS1 ~2.6V, APPS2 ~2.3V (based on Pedals.cpp init)

                     // So < 0.5V is definitely bad for both.

                }



                                // Ideally we should use the stored max from NVS/Pedals class, but Pedals::set_min updates min only.



                                // We'll update the class and NVS.



                

                

                Pedals::Get()->set_min(median1, median2);

                nvs_set_u16(nvs_storage_handle, "app1_stored_min", static_cast<uint16_t>(median1 * 100.0f));

                nvs_set_u16(nvs_storage_handle, "app2_stored_min", static_cast<uint16_t>(median2 * 100.0f));

                

                // Update CAN reporting signals

                apps1_min_ID2022.set(median1);

                apps2_min_ID2022.set(median2);

            }

            else if (calib_target == TARGET_MAX)

            {

                Pedals::Get()->set_max(median1, median2);

                nvs_set_u16(nvs_storage_handle, "app1_stored_max", static_cast<uint16_t>(median1 * 100.0f));

                nvs_set_u16(nvs_storage_handle, "app2_stored_max", static_cast<uint16_t>(median2 * 100.0f));



                // Update CAN reporting signals

                apps1_max_ID2022.set(median1);

                apps2_max_ID2022.set(median2);

            }



            calib_state = CALIB_IDLE;

            calib_target = TARGET_NONE;

        }

        break;

    }

}
