#include "RearECU.h"
#include "esp_log.h"
static const char* TAG = "RearECU"; //Used for ESP_LOGx commands. See ESP-IDF Documentation

using namespace RearECU;

void RearECU::rearECU_Task(void*)
{
    
    // remove uneeded CAN Rx messages
    CAN_Tx_10ms_IDs.erase(M192_COMMAND_MESSAGE);
    CAN_Tx_100ms_IDs.erase(BMS_CURRENT_LIMIT);
    CAN_Tx_100ms_IDs.erase(VCU_STATE_INFO);
    CAN_Tx_100ms_IDs.erase(VCU_FRONT_SENSORS_1);
    CAN_Tx_100ms_IDs.erase(VCU_FRONT_SENSORS_2);
    CAN_Tx_100ms_IDs.erase(VCU_FRONT_SENSORS_3);
    CAN_Tx_100ms_IDs.erase(VCU_FRONT_SENSORS_4);
    CAN_Tx_1000ms_IDs.erase(M192_COMMAND_MESSAGE);

    //add needed CAN messages
    CAN_Tx_100ms_IDs.insert(VCU_PDM_REAR);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_SENSORS_1);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_SENSORS_2);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_SENSORS_3);
    CAN_Tx_100ms_IDs.insert(VCU_REAR_SENSORS_4);
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

        // set HSD's based on CAN signals
        IO::Get()->HSDWrite(ECU_37_HSD1, HSD1_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_38_HSD2, HSD2_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_39_HSD3, HSD3_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_40_HSD4, HSD4_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_41_HSD5, HSD5_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_48_HSD6, HSD6_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_49_HSD7, HSD7_ID2012.get_bool());
        IO::Get()->HSDWrite(ECU_50_HSD8, HSD8_ID2012.get_bool());


        vTaskDelay(pdMS_TO_TICKS(100));

    }
}