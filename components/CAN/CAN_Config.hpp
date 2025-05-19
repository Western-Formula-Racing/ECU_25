#ifndef __CAN_CONFIG__
#define __CAN_CONFIG__
#include "CAN_Signal.hpp"
#include "etl/map.h"
#include "etl/vector.h"
#include "etl/set.h"

// Defining the CAN_IDs & Signals
#define VCU_PDM_REAR_CMD 2012
#define M173_MODULATION_AND_FLUX_INFO 173
#define M172_TORQUE_AND_TIMER_INFO 172
#define M194_READ_WRITE_PARAM_RESPONSE 194
#define M193_READ_WRITE_PARAM_COMMAND 193
#define M192_COMMAND_MESSAGE 192
#define M171_FAULT_CODES 171
#define M170_INTERNAL_STATES 170
#define M169_INTERNAL_VOLTAGES 169
#define M168_FLUX_ID_IQ_INFO 168
#define M167_VOLTAGE_INFO 167
#define M166_CURRENT_INFO 166
#define M165_MOTOR_POSITION_INFO 165
#define M164_DIGITAL_INPUT_STATUS 164
#define M163_ANALOG_INPUT_VOLTAGES 163
#define M162_TEMPERATURE_SET_3 162
#define M161_TEMPERATURE_SET_2 161
#define M160_TEMPERATURE_SET_1 160
#define M174_FIRMWARE_INFO 174
#define M175_DIAG_DATA_MESSAGE 175
#define BMS_CURRENT_LIMIT 514
#define M176_FAST_INFO 176
#define VCU_PEDAL_INFO 2000
#define VCU_PDM_FRONT 2001
#define VCU_STATE_INFO 2002
#define VCU_PRECHARGE 2003
#define VCU_PDM_REAR 2004
#define M177_TORQUE_CAPABILITY 177
#define PACKSTATUS 1056
#define ELCON_LIMITS 403105268
#define VCU_FRONT_SENSORS_1 2014
#define VCU_FRONT_SENSORS_2 2015
#define VCU_FRONT_SENSORS_3 2016
#define VCU_FRONT_SENSORS_4 2017
#define VCU_REAR_SENSORS_1 2018
#define VCU_REAR_SENSORS_2 2019
#define VCU_REAR_SENSORS_3 2020
#define VCU_REAR_SENSORS_4 2021
#define BMS_MIN_VCELL 999
#define VCU_APPS_CALIBRATION 2022
#define VCU_APPS_AUTOCALIBRATE 2023

inline CAN_Signal HSD1_ID2012{true, 0, 8};
inline CAN_Signal HSD2_ID2012{true, 8, 8};
inline CAN_Signal HSD3_ID2012{true, 16, 8};
inline CAN_Signal HSD4_ID2012{true, 24, 8};
inline CAN_Signal HSD5_ID2012{true, 32, 8};
inline CAN_Signal HSD6_ID2012{true, 40, 8};
inline CAN_Signal HSD7_ID2012{true, 48, 8};
inline CAN_Signal HSD8_ID2012{true, 56, 8};
inline CAN_Signal INV_Modulation_Index_ID173{true, 0, 16, 0.0001f};
inline CAN_Signal INV_Flux_Weakening_Output_ID173{true, 16, 16, 0.1f};
inline CAN_Signal INV_Id_Command_ID173{true, 32, 16, 0.1f};
inline CAN_Signal INV_Iq_Command_ID173{true, 48, 16, 0.1f};
inline CAN_Signal INV_Commanded_Torque_ID172{true, 0, 16, 0.1f};
inline CAN_Signal INV_Torque_Feedback_ID172{true, 16, 16, 0.1f};
inline CAN_Signal INV_Power_On_Timer_ID172{true, 32, 32, 0.003f};
inline CAN_Signal INV_Parameter_Response_Addr_ID194{true, 0, 16};
inline CAN_Signal INV_Parameter_Response_Write_OK_ID194{true, 16, 1};
inline CAN_Signal INV_Parameter_Response_Data_ID194{true, 32, 16};
inline CAN_Signal VCU_INV_Parameter_Address_ID193{true, 0, 16};
inline CAN_Signal VCU_INV_Parameter_RW_Command_ID193{true, 16, 1};
inline CAN_Signal VCU_INV_Parameter_Data_ID193{true, 32, 16};
inline CAN_Signal VCU_INV_Torque_Command_ID192{true, 0, 16, 0.1f};
inline CAN_Signal VCU_INV_Speed_Command_ID192{true, 16, 16};
inline CAN_Signal VCU_INV_Direction_Command_ID192{true, 32, 1};
inline CAN_Signal VCU_INV_Inverter_Enable_ID192{true, 40, 1};
inline CAN_Signal VCU_INV_Inverter_Discharge_ID192{true, 41, 1};
inline CAN_Signal VCU_INV_Speed_Mode_Enable_ID192{true, 42, 1};
inline CAN_Signal VCU_INV_Torque_Limit_Command_ID192{true, 48, 16, 0.1f};
inline CAN_Signal INV_Post_Fault_Lo_ID171{true, 0, 16};
inline CAN_Signal INV_Post_Fault_Hi_ID171{true, 16, 16};
inline CAN_Signal INV_Run_Fault_Lo_ID171{true, 32, 16};
inline CAN_Signal INV_Run_Fault_Hi_ID171{true, 48, 16};
inline CAN_Signal INV_VSM_State_ID170{true, 0, 8};
inline CAN_Signal INV_PWM_Frequency_ID170{true, 8, 8};
inline CAN_Signal INV_Inverter_State_ID170{true, 16, 8};
inline CAN_Signal INV_Relay_1_Status_ID170{true, 24, 1};
inline CAN_Signal INV_Relay_2_Status_ID170{true, 25, 1};
inline CAN_Signal INV_Relay_3_Status_ID170{true, 26, 1};
inline CAN_Signal INV_Relay_4_Status_ID170{true, 27, 1};
inline CAN_Signal INV_Relay_5_Status_ID170{true, 28, 1};
inline CAN_Signal INV_Relay_6_Status_ID170{true, 29, 1};
inline CAN_Signal INV_Inverter_Run_Mode_ID170{true, 32, 1};
inline CAN_Signal INV_Self_Sensing_Assist_Enable_ID170{true, 33, 1};
inline CAN_Signal INV_Inverter_Discharge_State_ID170{true, 37, 3};
inline CAN_Signal INV_Inverter_Command_Mode_ID170{true, 40, 1};
inline CAN_Signal INV_Rolling_Counter_ID170{true, 44, 4};
inline CAN_Signal INV_Inverter_Enable_State_ID170{true, 48, 1};
inline CAN_Signal INV_Burst_Model_Mode_ID170{true, 49, 1};
inline CAN_Signal INV_Key_Switch_Start_Status_ID170{true, 54, 1};
inline CAN_Signal INV_Inverter_Enable_Lockout_ID170{true, 55, 1};
inline CAN_Signal INV_Direction_Command_ID170{true, 56, 1};
inline CAN_Signal INV_BMS_Active_ID170{true, 57, 1};
inline CAN_Signal INV_BMS_Torque_Limiting_ID170{true, 58, 1};
inline CAN_Signal INV_Limit_Max_Speed_ID170{true, 59, 1};
inline CAN_Signal INV_Limit_Hot_Spot_ID170{true, 60, 1};
inline CAN_Signal INV_Low_Speed_Limiting_ID170{true, 61, 1};
inline CAN_Signal INV_Limit_Coolant_Derating_ID170{true, 62, 1};
inline CAN_Signal INV_Limit_Stall_Burst_Model_ID170{true, 63, 1};
inline CAN_Signal INV_Ref_Voltage_1_5_ID169{true, 0, 16, 0.01f};
inline CAN_Signal INV_Ref_Voltage_2_5_ID169{true, 16, 16, 0.01f};
inline CAN_Signal INV_Ref_Voltage_5_0_ID169{true, 32, 16, 0.01f};
inline CAN_Signal INV_Ref_Voltage_12_0_ID169{true, 48, 16, 0.01f};
inline CAN_Signal INV_Vd_ff_ID168{true, 0, 16, 0.1f};
inline CAN_Signal INV_Vq_ff_ID168{true, 16, 16, 0.1f};
inline CAN_Signal INV_Id_ID168{true, 32, 16, 0.1f};
inline CAN_Signal INV_Iq_ID168{true, 48, 16, 0.1f};
inline CAN_Signal INV_DC_Bus_Voltage_ID167{true, 0, 16, 0.1f};
inline CAN_Signal INV_Output_Voltage_ID167{true, 16, 16, 0.1f};
inline CAN_Signal INV_VAB_Vd_Voltage_ID167{true, 32, 16, 0.1f};
inline CAN_Signal INV_VBC_Vq_Voltage_ID167{true, 48, 16, 0.1f};
inline CAN_Signal INV_Phase_A_Current_ID166{true, 0, 16, 0.1f};
inline CAN_Signal INV_Phase_B_Current_ID166{true, 16, 16, 0.1f};
inline CAN_Signal INV_Phase_C_Current_ID166{true, 32, 16, 0.1f};
inline CAN_Signal INV_DC_Bus_Current_ID166{true, 48, 16, 0.1f};
inline CAN_Signal INV_Motor_Angle_Electrical_ID165{true, 0, 16, 0.1f};
inline CAN_Signal INV_Motor_Speed_ID165{true, 16, 16};
inline CAN_Signal INV_Electrical_Output_Frequency_ID165{true, 32, 16, 0.1f};
inline CAN_Signal INV_Delta_Resolver_Filtered_ID165{true, 48, 16, 0.1f};
inline CAN_Signal INV_Digital_Input_1_ID164{true, 0, 1};
inline CAN_Signal INV_Digital_Input_2_ID164{true, 8, 1};
inline CAN_Signal INV_Digital_Input_3_ID164{true, 16, 1};
inline CAN_Signal INV_Digital_Input_4_ID164{true, 24, 1};
inline CAN_Signal INV_Digital_Input_5_ID164{true, 32, 1};
inline CAN_Signal INV_Digital_Input_6_ID164{true, 40, 1};
inline CAN_Signal INV_Digital_Input_7_ID164{true, 48, 1};
inline CAN_Signal INV_Digital_Input_8_ID164{true, 56, 1};
inline CAN_Signal INV_Analog_Input_1_ID163{true, 0, 10, 0.01f};
inline CAN_Signal INV_Analog_Input_2_ID163{true, 10, 10, 0.01f};
inline CAN_Signal INV_Analog_Input_3_ID163{true, 20, 10, 0.01f};
inline CAN_Signal INV_Analog_Input_4_ID163{true, 32, 10, 0.01f};
inline CAN_Signal INV_Analog_Input_5_ID163{true, 42, 10, 0.01f};
inline CAN_Signal INV_Analog_Input_6_ID163{true, 52, 10, 0.01f};
inline CAN_Signal INV_Coolant_Temp_ID162{true, 0, 16, 0.1f};
inline CAN_Signal INV_Hot_Spot_Temp_ID162{true, 16, 16, 0.1f};
inline CAN_Signal INV_Motor_Temp_ID162{true, 32, 16, 0.1f};
inline CAN_Signal INV_Torque_Shudder_ID162{true, 48, 16, 0.1f};
inline CAN_Signal INV_Control_Board_Temp_ID161{true, 0, 16, 0.1f};
inline CAN_Signal INV_RTD1_Temperature_ID161{true, 16, 16, 0.1f};
inline CAN_Signal INV_RTD2_Temperature_ID161{true, 32, 16, 0.1f};
inline CAN_Signal INV_Stall_Burst_Model_Temp_ID161{true, 48, 16, 0.1f};
inline CAN_Signal INV_Module_A_Temp_ID160{true, 0, 16, 0.1f};
inline CAN_Signal INV_Module_B_Temp_ID160{true, 16, 16, 0.1f};
inline CAN_Signal INV_Module_C_Temp_ID160{true, 32, 16, 0.1f};
inline CAN_Signal INV_Gate_Driver_Board_Temp_ID160{true, 48, 16, 0.1f};
inline CAN_Signal INV_Project_Code_EEP_Ver_ID174{true, 0, 16};
inline CAN_Signal INV_SW_Version_ID174{true, 16, 16};
inline CAN_Signal INV_DateCode_MMDD_ID174{true, 32, 16};
inline CAN_Signal INV_DateCode_YYYY_ID174{true, 48, 16};
inline CAN_Signal INV_Diag_Record_ID175{true, 0, 8};
inline CAN_Signal INV_Diag_Segment_ID175{true, 8, 8};
inline CAN_Signal INV_Diag_Gamma_Resolver_ID175{true, 16, 16, 0.1f};
inline CAN_Signal INV_Diag_Cos_Used_ID175{true, 16, 16};
inline CAN_Signal INV_Diag_Ic_ID175{true, 16, 16, 0.1f};
inline CAN_Signal INV_Diag_Id_cmd_ID175{true, 16, 16, 0.1f};
inline CAN_Signal INV_Diag_Vq_Cmd_ID175{true, 16, 16, 0.1f};
inline CAN_Signal INV_Diag_PWM_Freq_ID175{true, 16, 16};
inline CAN_Signal INV_Diag_Gamma_Observer_ID175{true, 32, 16, 0.1f};
inline CAN_Signal INV_Diag_Ia_ID175{true, 32, 16, 0.1f};
inline CAN_Signal INV_Diag_Vdc_ID175{true, 32, 16, 0.1f};
inline CAN_Signal INV_Diag_Mod_Index_ID175{true, 32, 16, 0.0001f};
inline CAN_Signal INV_Diag_Vd_Cmd_ID175{true, 32, 16, 0.1f};
inline CAN_Signal INV_Diag_Run_Faults_Lo_ID175{true, 32, 16};
inline CAN_Signal INV_Diag_Sin_Used_ID175{true, 48, 16};
inline CAN_Signal INV_Diag_Ib_ID175{true, 48, 16, 0.1f};
inline CAN_Signal INV_Diag_Iq_cmd_ID175{true, 48, 16, 0.1f};
inline CAN_Signal INV_Diag_FW_Output_ID175{true, 48, 16, 0.1f};
inline CAN_Signal INV_Diag_Vqs_Cmd_ID175{true, 48, 16, 0.1f};
inline CAN_Signal INV_Diag_Run_Faults_Hi_ID175{true, 48, 16};
inline CAN_Signal BMS_Max_Discharge_Current_ID514{true, 0, 16};
inline CAN_Signal BMS_Max_Charge_Current_ID514{true, 16, 16};
inline CAN_Signal INV_Fast_Torque_Command_ID176{true, 0, 16, 0.1f};
inline CAN_Signal INV_Fast_Torque_Feedback_ID176{true, 16, 16, 0.1f};
inline CAN_Signal INV_Fast_Motor_Speed_ID176{true, 32, 16};
inline CAN_Signal INV_Fast_DC_Bus_Voltage_ID176{true, 48, 16, 0.1f};
inline CAN_Signal sensorPlausibility_ID2000{true, 0, 1};
inline CAN_Signal pedalPosition_ID2000{true, 8, 8};
inline CAN_Signal sensor1Position_ID2000{true, 16, 8};
inline CAN_Signal sensor2Position_ID2000{true, 24, 8};
inline CAN_Signal brakePressure1Signal_ID2000{true, 32, 16, 0.01f};
inline CAN_Signal brakePressure2Signal_ID2000{true, 48, 16, 0.01f};
inline CAN_Signal HSD1_ID2001{true, 0, 1};
inline CAN_Signal HSD2_ID2001{true, 1, 1};
inline CAN_Signal HSD3_ID2001{true, 2, 1};
inline CAN_Signal HSD4_ID2001{true, 3, 1};
inline CAN_Signal HSD5_ID2001{true, 4, 1};
inline CAN_Signal HSD6_ID2001{true, 5, 1};
inline CAN_Signal HSD7_ID2001{true, 6, 1};
inline CAN_Signal HSD8_ID2001{true, 7, 1};
inline CAN_Signal RTD_Button_ID2002{true, 0, 1};
inline CAN_Signal State_ID2002{true, 8, 8};
inline CAN_Signal Precharge_Enable_ID2003{true, 0, 1};
inline CAN_Signal Precharge_OK_ID2003{true, 1, 1};
inline CAN_Signal HSD1_ID2004{true, 0, 1};
inline CAN_Signal HSD2_ID2004{true, 1, 1};
inline CAN_Signal HSD3_ID2004{true, 2, 1};
inline CAN_Signal HSD4_ID2004{true, 3, 1};
inline CAN_Signal HSD5_ID2004{true, 4, 1};
inline CAN_Signal HSD6_ID2004{true, 5, 1};
inline CAN_Signal HSD7_ID2004{true, 6, 1};
inline CAN_Signal HSD8_ID2004{true, 7, 1};
inline CAN_Signal INV_Torque_Capability_ID177{true, 0, 16, 0.1f};
inline CAN_Signal PackCurrent_ID1056{true, 0, 16, 0.1f};
inline CAN_Signal IMDRelay_ID1056{true, 16, 1};
inline CAN_Signal AMSRelay_ID1056{true, 17, 1};
inline CAN_Signal BSPDRelay_ID1056{true, 18, 1};
inline CAN_Signal LatchRelay_ID1056{true, 19, 1};
inline CAN_Signal Safetyloop_return_ID1056{true, 20, 1};
inline CAN_Signal HV_Active_ID1056{true, 21, 1};
inline CAN_Signal SOC_ID1056{true, 24, 16, 0.01f};
inline CAN_Signal PackStatus_ID1056{true, 40, 8};
inline CAN_Signal Fault_ID1056{true, 48, 8};
inline CAN_Signal Max_charge_voltage_ID403105268{false, 7, 16, 0.1f};
inline CAN_Signal Max_charge_current_ID403105268{false, 23, 16, 0.1f};
inline CAN_Signal Control_ID403105268{true, 32, 8};
inline CAN_Signal A1_ID2014{true, 0, 16, 0.01f};
inline CAN_Signal A2_ID2014{true, 16, 16, 0.01f};
inline CAN_Signal A3_ID2014{true, 32, 16, 0.01f};
inline CAN_Signal A4_ID2014{true, 48, 16, 0.01f};
inline CAN_Signal A5_ID2015{true, 0, 16, 0.01f};
inline CAN_Signal A6_ID2015{true, 16, 16, 0.01f};
inline CAN_Signal A7_ID2015{true, 32, 16, 0.01f};
inline CAN_Signal A8_ID2015{true, 48, 16, 0.01f};
inline CAN_Signal A9_ID2016{true, 0, 16, 0.01f};
inline CAN_Signal A10_ID2016{true, 16, 16, 0.01f};
inline CAN_Signal A11_ID2016{true, 32, 16, 0.01f};
inline CAN_Signal A12_ID2016{true, 48, 16, 0.01f};
inline CAN_Signal A13_ID2017{true, 0, 16, 0.01f};
inline CAN_Signal A14_ID2017{true, 16, 16, 0.01f};
inline CAN_Signal A15_ID2017{true, 32, 16, 0.01f};
inline CAN_Signal A16_ID2017{true, 48, 16, 0.01f};
inline CAN_Signal A1_ID2018{true, 0, 16, 0.01f};
inline CAN_Signal A2_ID2018{true, 16, 16, 0.01f};
inline CAN_Signal A3_ID2018{true, 32, 16, 0.01f};
inline CAN_Signal A4_ID2018{true, 48, 16, 0.01f};
inline CAN_Signal A5_ID2019{true, 0, 16, 0.01f};
inline CAN_Signal A6_ID2019{true, 16, 16, 0.01f};
inline CAN_Signal A7_ID2019{true, 32, 16, 0.01f};
inline CAN_Signal A8_ID2019{true, 48, 16, 0.01f};
inline CAN_Signal A9_ID2020{true, 0, 16, 0.01f};
inline CAN_Signal A10_ID2020{true, 16, 16, 0.01f};
inline CAN_Signal A11_ID2020{true, 32, 16, 0.01f};
inline CAN_Signal A12_ID2020{true, 48, 16, 0.01f};
inline CAN_Signal A13_ID2021{true, 0, 16, 0.01f};
inline CAN_Signal A14_ID2021{true, 16, 16, 0.01f};
inline CAN_Signal A15_ID2021{true, 32, 16, 0.01f};
inline CAN_Signal A16_ID2021{true, 48, 16, 0.01f};
inline CAN_Signal Absolute_min_cell_voltage_ID999{true, 0, 16, 0.0001f};
inline CAN_Signal apps1_min_ID2022{true, 0, 16, 0.01f};
inline CAN_Signal apps1_max_ID2022{true, 16, 16, 0.01f};
inline CAN_Signal apps2_min_ID2022{true, 32, 16, 0.01f};
inline CAN_Signal apps2_max_ID2022{true, 48, 16, 0.01f};
inline CAN_Signal set_min_ID2023{true, 0, 8, 1.0f};
inline CAN_Signal set_max_ID2023{true, 8, 16, 1.0f};

// Define the CAN Map
inline etl::map CAN_Map
{ 
    etl::pair{VCU_PDM_REAR_CMD, etl::vector<CAN_Signal*, 16>{ 
        &HSD1_ID2012,
        &HSD2_ID2012,
        &HSD3_ID2012,
        &HSD4_ID2012,
        &HSD5_ID2012,
        &HSD6_ID2012,
        &HSD7_ID2012,
        &HSD8_ID2012 
        } },
    etl::pair{M173_MODULATION_AND_FLUX_INFO, etl::vector<CAN_Signal*, 16>{ 
        &INV_Modulation_Index_ID173,
        &INV_Flux_Weakening_Output_ID173,
        &INV_Id_Command_ID173,
        &INV_Iq_Command_ID173 
        } },
    etl::pair{M172_TORQUE_AND_TIMER_INFO, etl::vector<CAN_Signal*, 16>{ 
        &INV_Commanded_Torque_ID172,
        &INV_Torque_Feedback_ID172,
        &INV_Power_On_Timer_ID172 
        } },
    etl::pair{M194_READ_WRITE_PARAM_RESPONSE, etl::vector<CAN_Signal*, 16>{ 
        &INV_Parameter_Response_Addr_ID194,
        &INV_Parameter_Response_Write_OK_ID194,
        &INV_Parameter_Response_Data_ID194 
        } },
    etl::pair{M193_READ_WRITE_PARAM_COMMAND, etl::vector<CAN_Signal*, 16>{ 
        &VCU_INV_Parameter_Address_ID193,
        &VCU_INV_Parameter_RW_Command_ID193,
        &VCU_INV_Parameter_Data_ID193 
        } },
    etl::pair{M192_COMMAND_MESSAGE, etl::vector<CAN_Signal*, 16>{ 
        &VCU_INV_Torque_Command_ID192,
        &VCU_INV_Speed_Command_ID192,
        &VCU_INV_Direction_Command_ID192,
        &VCU_INV_Inverter_Enable_ID192,
        &VCU_INV_Inverter_Discharge_ID192,
        &VCU_INV_Speed_Mode_Enable_ID192,
        &VCU_INV_Torque_Limit_Command_ID192 
        } },
    etl::pair{M171_FAULT_CODES, etl::vector<CAN_Signal*, 16>{ 
        &INV_Post_Fault_Lo_ID171,
        &INV_Post_Fault_Hi_ID171,
        &INV_Run_Fault_Lo_ID171,
        &INV_Run_Fault_Hi_ID171 
        } },
    etl::pair{M170_INTERNAL_STATES, etl::vector<CAN_Signal*, 16>{ 
        &INV_VSM_State_ID170,
        &INV_PWM_Frequency_ID170,
        &INV_Inverter_State_ID170,
        &INV_Relay_1_Status_ID170,
        &INV_Relay_2_Status_ID170,
        &INV_Relay_3_Status_ID170,
        &INV_Relay_4_Status_ID170,
        &INV_Relay_5_Status_ID170,
        &INV_Relay_6_Status_ID170,
        &INV_Inverter_Run_Mode_ID170,
        &INV_Self_Sensing_Assist_Enable_ID170,
        &INV_Inverter_Discharge_State_ID170,
        &INV_Inverter_Command_Mode_ID170,
        &INV_Rolling_Counter_ID170,
        &INV_Inverter_Enable_State_ID170,
        &INV_Burst_Model_Mode_ID170,
        &INV_Key_Switch_Start_Status_ID170,
        &INV_Inverter_Enable_Lockout_ID170,
        &INV_Direction_Command_ID170,
        &INV_BMS_Active_ID170,
        &INV_BMS_Torque_Limiting_ID170,
        &INV_Limit_Max_Speed_ID170,
        &INV_Limit_Hot_Spot_ID170,
        &INV_Low_Speed_Limiting_ID170,
        &INV_Limit_Coolant_Derating_ID170,
        &INV_Limit_Stall_Burst_Model_ID170 
        } },
    etl::pair{M169_INTERNAL_VOLTAGES, etl::vector<CAN_Signal*, 16>{ 
        &INV_Ref_Voltage_1_5_ID169,
        &INV_Ref_Voltage_2_5_ID169,
        &INV_Ref_Voltage_5_0_ID169,
        &INV_Ref_Voltage_12_0_ID169 
        } },
    etl::pair{M168_FLUX_ID_IQ_INFO, etl::vector<CAN_Signal*, 16>{ 
        &INV_Vd_ff_ID168,
        &INV_Vq_ff_ID168,
        &INV_Id_ID168,
        &INV_Iq_ID168 
        } },
    etl::pair{M167_VOLTAGE_INFO, etl::vector<CAN_Signal*, 16>{ 
        &INV_DC_Bus_Voltage_ID167,
        &INV_Output_Voltage_ID167,
        &INV_VAB_Vd_Voltage_ID167,
        &INV_VBC_Vq_Voltage_ID167 
        } },
    etl::pair{M166_CURRENT_INFO, etl::vector<CAN_Signal*, 16>{ 
        &INV_Phase_A_Current_ID166,
        &INV_Phase_B_Current_ID166,
        &INV_Phase_C_Current_ID166,
        &INV_DC_Bus_Current_ID166 
        } },
    etl::pair{M165_MOTOR_POSITION_INFO, etl::vector<CAN_Signal*, 16>{ 
        &INV_Motor_Angle_Electrical_ID165,
        &INV_Motor_Speed_ID165,
        &INV_Electrical_Output_Frequency_ID165,
        &INV_Delta_Resolver_Filtered_ID165 
        } },
    etl::pair{M164_DIGITAL_INPUT_STATUS, etl::vector<CAN_Signal*, 16>{ 
        &INV_Digital_Input_1_ID164,
        &INV_Digital_Input_2_ID164,
        &INV_Digital_Input_3_ID164,
        &INV_Digital_Input_4_ID164,
        &INV_Digital_Input_5_ID164,
        &INV_Digital_Input_6_ID164,
        &INV_Digital_Input_7_ID164,
        &INV_Digital_Input_8_ID164 
        } },
    etl::pair{M163_ANALOG_INPUT_VOLTAGES, etl::vector<CAN_Signal*, 16>{ 
        &INV_Analog_Input_1_ID163,
        &INV_Analog_Input_2_ID163,
        &INV_Analog_Input_3_ID163,
        &INV_Analog_Input_4_ID163,
        &INV_Analog_Input_5_ID163,
        &INV_Analog_Input_6_ID163 
        } },
    etl::pair{M162_TEMPERATURE_SET_3, etl::vector<CAN_Signal*, 16>{ 
        &INV_Coolant_Temp_ID162,
        &INV_Hot_Spot_Temp_ID162,
        &INV_Motor_Temp_ID162,
        &INV_Torque_Shudder_ID162 
        } },
    etl::pair{M161_TEMPERATURE_SET_2, etl::vector<CAN_Signal*, 16>{ 
        &INV_Control_Board_Temp_ID161,
        &INV_RTD1_Temperature_ID161,
        &INV_RTD2_Temperature_ID161,
        &INV_Stall_Burst_Model_Temp_ID161 
        } },
    etl::pair{M160_TEMPERATURE_SET_1, etl::vector<CAN_Signal*, 16>{ 
        &INV_Module_A_Temp_ID160,
        &INV_Module_B_Temp_ID160,
        &INV_Module_C_Temp_ID160,
        &INV_Gate_Driver_Board_Temp_ID160 
        } },
    etl::pair{M174_FIRMWARE_INFO, etl::vector<CAN_Signal*, 16>{ 
        &INV_Project_Code_EEP_Ver_ID174,
        &INV_SW_Version_ID174,
        &INV_DateCode_MMDD_ID174,
        &INV_DateCode_YYYY_ID174 
        } },
    etl::pair{M175_DIAG_DATA_MESSAGE, etl::vector<CAN_Signal*, 16>{ 
        &INV_Diag_Record_ID175,
        &INV_Diag_Segment_ID175,
        &INV_Diag_Gamma_Resolver_ID175,
        &INV_Diag_Cos_Used_ID175,
        &INV_Diag_Ic_ID175,
        &INV_Diag_Id_cmd_ID175,
        &INV_Diag_Vq_Cmd_ID175,
        &INV_Diag_PWM_Freq_ID175,
        &INV_Diag_Gamma_Observer_ID175,
        &INV_Diag_Ia_ID175,
        &INV_Diag_Vdc_ID175,
        &INV_Diag_Mod_Index_ID175,
        &INV_Diag_Vd_Cmd_ID175,
        &INV_Diag_Run_Faults_Lo_ID175,
        &INV_Diag_Sin_Used_ID175,
        &INV_Diag_Ib_ID175,
        &INV_Diag_Iq_cmd_ID175,
        &INV_Diag_FW_Output_ID175,
        &INV_Diag_Vqs_Cmd_ID175,
        &INV_Diag_Run_Faults_Hi_ID175 
        } },
    etl::pair{BMS_CURRENT_LIMIT, etl::vector<CAN_Signal*, 16>{ 
        &BMS_Max_Discharge_Current_ID514,
        &BMS_Max_Charge_Current_ID514 
        } },
    etl::pair{M176_FAST_INFO, etl::vector<CAN_Signal*, 16>{ 
        &INV_Fast_Torque_Command_ID176,
        &INV_Fast_Torque_Feedback_ID176,
        &INV_Fast_Motor_Speed_ID176,
        &INV_Fast_DC_Bus_Voltage_ID176 
        } },
    etl::pair{VCU_PEDAL_INFO, etl::vector<CAN_Signal*, 16>{ 
        &sensorPlausibility_ID2000,
        &pedalPosition_ID2000,
        &sensor1Position_ID2000,
        &sensor2Position_ID2000,
        &brakePressure1Signal_ID2000,
        &brakePressure2Signal_ID2000 
        } },
    etl::pair{VCU_PDM_FRONT, etl::vector<CAN_Signal*, 16>{ 
        &HSD1_ID2001,
        &HSD2_ID2001,
        &HSD3_ID2001,
        &HSD4_ID2001,
        &HSD5_ID2001,
        &HSD6_ID2001,
        &HSD7_ID2001,
        &HSD8_ID2001 
        } },
    etl::pair{VCU_STATE_INFO, etl::vector<CAN_Signal*, 16>{ 
        &RTD_Button_ID2002,
        &State_ID2002 
        } },
    etl::pair{VCU_PRECHARGE, etl::vector<CAN_Signal*, 16>{ 
        &Precharge_Enable_ID2003,
        &Precharge_OK_ID2003 
        } },
    etl::pair{VCU_PDM_REAR, etl::vector<CAN_Signal*, 16>{ 
        &HSD1_ID2004,
        &HSD2_ID2004,
        &HSD3_ID2004,
        &HSD4_ID2004,
        &HSD5_ID2004,
        &HSD6_ID2004,
        &HSD7_ID2004,
        &HSD8_ID2004 
        } },
    etl::pair{M177_TORQUE_CAPABILITY, etl::vector<CAN_Signal*, 16>{ 
        &INV_Torque_Capability_ID177 
        } },
    etl::pair{PACKSTATUS, etl::vector<CAN_Signal*, 16>{ 
        &PackCurrent_ID1056,
        &IMDRelay_ID1056,
        &AMSRelay_ID1056,
        &BSPDRelay_ID1056,
        &LatchRelay_ID1056,
        &Safetyloop_return_ID1056,
        &HV_Active_ID1056,
        &SOC_ID1056,
        &PackStatus_ID1056,
        &Fault_ID1056 
        } },
    etl::pair{ELCON_LIMITS, etl::vector<CAN_Signal*, 16>{ 
        &Max_charge_voltage_ID403105268,
        &Max_charge_current_ID403105268,
        &Control_ID403105268 
        } },
    etl::pair{VCU_FRONT_SENSORS_1, etl::vector<CAN_Signal*, 16>{ 
        &A1_ID2014,
        &A2_ID2014,
        &A3_ID2014,
        &A4_ID2014 
        } },
    etl::pair{VCU_FRONT_SENSORS_2, etl::vector<CAN_Signal*, 16>{ 
        &A5_ID2015,
        &A6_ID2015,
        &A7_ID2015,
        &A8_ID2015 
        } },
    etl::pair{VCU_FRONT_SENSORS_3, etl::vector<CAN_Signal*, 16>{ 
        &A9_ID2016,
        &A10_ID2016,
        &A11_ID2016,
        &A12_ID2016 
        } },
    etl::pair{VCU_FRONT_SENSORS_4, etl::vector<CAN_Signal*, 16>{ 
        &A13_ID2017,
        &A14_ID2017,
        &A15_ID2017,
        &A16_ID2017 
        } },
    etl::pair{VCU_REAR_SENSORS_1, etl::vector<CAN_Signal*, 16>{ 
        &A1_ID2018,
        &A2_ID2018,
        &A3_ID2018,
        &A4_ID2018 
        } },
    etl::pair{VCU_REAR_SENSORS_2, etl::vector<CAN_Signal*, 16>{ 
        &A5_ID2019,
        &A6_ID2019,
        &A7_ID2019,
        &A8_ID2019 
        } },
    etl::pair{VCU_REAR_SENSORS_3, etl::vector<CAN_Signal*, 16>{ 
        &A9_ID2020,
        &A10_ID2020,
        &A11_ID2020,
        &A12_ID2020 
        } },
    etl::pair{VCU_REAR_SENSORS_4, etl::vector<CAN_Signal*, 16>{ 
        &A13_ID2021,
        &A14_ID2021,
        &A15_ID2021,
        &A16_ID2021 
        } },
    etl::pair{BMS_MIN_VCELL, etl::vector<CAN_Signal*, 16>{ 
        &Absolute_min_cell_voltage_ID999 
        } },
    etl::pair{VCU_APPS_CALIBRATION, etl::vector<CAN_Signal*, 16>{ 
        &apps1_min_ID2022,
        &apps1_max_ID2022,
        &apps2_min_ID2022,
        &apps2_max_ID2022 
        } },
    etl::pair{VCU_APPS_AUTOCALIBRATE, etl::vector<CAN_Signal*, 16>{ 
        &set_min_ID2023,
        &set_max_ID2023,
        } },
};

inline etl::set CAN_Rx_IDs = {2023,2022,173,172,194,171,170,169,168,167,166,165,164,163,162,161,160,174,175,514,176,1712,1713,1714,406451072,406451073,406451074,406451075,406451076,177,1000,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1023,1024,1025,1026,1027,1028,1029,1030,1031,1032,1033,1034,1035,1036,1037,1038,1039,1040,1041,1042,1043,1044,1045,1046,1047,1048,1049,1050,1051,1052,1053,1054,1055,1056,403105268,419385573,1001,1002,1003,1004,1005,2012};

inline etl::set CAN_Tx_10ms_IDs = {M192_COMMAND_MESSAGE};
inline etl::set CAN_Tx_100ms_IDs = {BMS_CURRENT_LIMIT, VCU_STATE_INFO, VCU_FRONT_SENSORS_1,VCU_FRONT_SENSORS_2, VCU_FRONT_SENSORS_3, VCU_FRONT_SENSORS_4, VCU_PDM_REAR_CMD};
inline etl::set CAN_Tx_1000ms_IDs = {192};;
//they don't like to be empty 

#endif 
