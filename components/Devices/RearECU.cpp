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
    for(;;){
        // poll sensors
        Sensors::Get()->poll_sensors();
        A1_ID2018.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(0)));
        A2_ID2018.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(1)));
        A3_ID2018.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(2)));
        A4_ID2018.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(3)));
        A5_ID2019.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(4)));
        A6_ID2019.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(5)));
        A7_ID2019.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(6)));
        A8_ID2019.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(7)));
        A9_ID2020.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(8)));
        A10_ID2020.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(9)));
        A11_ID2020.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(10)));
        A12_ID2020.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(11)));
        A13_ID2021.set(Sensors::Get()->get_sensor_voltage(static_cast<Sensors::SENSOR_INDEX>(12)));
        printf(">HSD1:%d\n", HSD1_ID2012.get_bool());
        printf(">HSD2:%d\n", HSD2_ID2012.get_bool());
        printf(">HSD3:%d\n", HSD3_ID2012.get_bool());
        printf(">HSD4:%d\n", HSD4_ID2012.get_bool());
        printf(">HSD5:%d\n", HSD5_ID2012.get_bool());
        printf(">HSD6:%d\n", HSD6_ID2012.get_bool());
        printf(">HSD7:%d\n", HSD7_ID2012.get_bool());
        printf(">HSD8:%d\n", HSD8_ID2012.get_bool());
        
        // set HSD's based on CAN signals
        IO::Get()->HSDWrite(ECU_37_HSD1, HSD1_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_38_HSD2, HSD2_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_39_HSD3, HSD3_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_40_HSD4, HSD4_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_41_HSD5, HSD5_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_48_HSD6, HSD6_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_49_HSD7, HSD7_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_50_HSD8, HSD8_ID2012.get_bool());

        //set IMU Signals
        Accel_X_ID2026.set(IO::Get()->getAccelX());
        Accel_Y_ID2026.set(IO::Get()->getAccelY());
        Accel_Z_ID2026.set(IO::Get()->getAccelZ());
        Gyro_X_ID2027.set(IO::Get()->getGyroX());
        Gyro_Y_ID2027.set(IO::Get()->getGyroY());
        Gyro_Z_ID2027.set(IO::Get()->getGyroZ());

        vTaskDelay(pdMS_TO_TICKS(100));

    }
}