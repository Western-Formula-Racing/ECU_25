#include "RearECU.h"
#include "esp_log.h"
static const char* TAG = "RearECU"; //Used for ESP_LOGx commands. See ESP-IDF Documentation

using namespace RearECU;

void RearECU::rearECU_Task(void*)
{
    
    // remove uneeded CAN Tx messages
    CAN_Tx_10ms_IDs.clear();
    CAN_Tx_100ms_IDs.clear();
    CAN_Tx_1000ms_IDs.clear();

    //add needed CAN messages
    CAN_Tx_100ms_IDs.insert(VCU_PDM_REAR);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_SENSORS_1);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_SENSORS_2);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_SENSORS_3);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_SENSORS_4);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_IMU_1);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_IMU_2);

    pwm_init();
    int pump_duty_cycle = 100; 



    for(;;){
        // poll sensors
        Sensors::Get()->poll_sensors();
        Rear_A1_ID2018.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(0)));
        Rear_A2_ID2018.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(1)));
        Rear_A3_ID2018.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(2)));
        Rear_A4_ID2018.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(3)));
        Rear_A5_ID2019.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(4)));
        Rear_A6_ID2019.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(5)));
        Rear_A7_ID2019.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(6)));
        Rear_A8_ID2019.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(7)));
        Rear_A9_ID2020.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(8)));
        Rear_A10_ID2020.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(9)));
        Rear_A11_ID2020.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(10)));
        Rear_A12_ID2020.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(11)));
        Rear_A13_ID2021.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(12)));
        printf(">HSD1:%d\n", Rear_Cmd_HSD1_ID2012.get_bool());
        printf(">HSD2:%d\n", Rear_Cmd_HSD2_ID2012.get_bool());
        printf(">HSD3:%d\n", Rear_Cmd_HSD3_ID2012.get_bool());
        printf(">HSD4:%d\n", Rear_Cmd_HSD4_ID2012.get_bool());
        printf(">HSD5:%d\n", Rear_Cmd_HSD5_ID2012.get_bool());
        printf(">HSD6:%d\n", Rear_Cmd_HSD6_ID2012.get_bool());
        printf(">HSD7:%d\n", Rear_Cmd_HSD7_ID2012.get_bool());
        printf(">HSD8:%d\n", Rear_Cmd_HSD8_ID2012.get_bool());
        
        // set HSD's based on CAN signals
        IO::Get()->HSDWrite(ECU_37_HSD1, Rear_Cmd_HSD1_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_38_HSD2, Rear_Cmd_HSD2_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_39_HSD3, Rear_Cmd_HSD3_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_40_HSD4, Rear_Cmd_HSD4_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_41_HSD5, Rear_Cmd_HSD5_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_48_HSD6, Rear_Cmd_HSD6_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_49_HSD7, Rear_Cmd_HSD7_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_50_HSD8, Rear_Cmd_HSD8_ID2012.get_bool());

        //set IMU Signals
        Rear_Accel_X_ID2026.set(IO::Get()->getAccelX());
        Rear_Accel_Y_ID2026.set(IO::Get()->getAccelY());
        Rear_Accel_Z_ID2026.set(IO::Get()->getAccelZ());
        Rear_Gyro_X_ID2027.set(IO::Get()->getGyroX());
        Rear_Gyro_Y_ID2027.set(IO::Get()->getGyroY());
        Rear_Gyro_Z_ID2027.set(IO::Get()->getGyroZ());

        // dynamic pump pwm for cooling
        //25% bias, then ramp up to 60 degrees for max
        pump_duty_cycle =  25 +  (75*INV_Coolant_Temp_ID162.get_float()/60);
        printf(">pump_duty_cycle:%d\n", pump_duty_cycle);
        pwm_set_duty(pump_duty_cycle);

        vTaskDelay(pdMS_TO_TICKS(100));

    }
}