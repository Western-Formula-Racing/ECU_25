#include "BMS.h"
#include "esp_log.h"
static const char *TAG = "BMS"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

BMS *BMS::instancePtr = nullptr;
SemaphoreHandle_t BMS::mutex = xSemaphoreCreateMutex();

enum error_e {
  NO_ERROR = 0,
  // Shared between TORCH and Mobo
  OVERTEMP = 69,  //Thermistor > 60C
  UNDERVOLTAGE = 70,   //Cell voltage < 3V
  OVERVOLTAGE = 71,    //cell voltage > 4.2 V
  OPENCELL = 72,       //open cell circuit
  OPENTHERMISTOR = 73, //open thermistor circuit
  ERROR_DIAGN = 74,          //LTC6813 DIAGN fail
  ERROR_MUTE = 75,
  ERROR_CVST = 76,           //LTC6813 CVST fail
  ERROR_STATST = 77,         //LTC6813 STATST fail
  ERROR_AXST = 78,           //LTC6813 AXST fail
  ERROR_ADOL = 79,           //LTC6813 ADOL fail
  ERROR_OUT_OF_RANGE_VA = 80,
  ERROR_OUT_OF_RANGE_VD = 81,
  ERROR_OUT_OF_RANGE_REF2 = 82,
  ERROR_LTC6813_OVERHEAT = 83,
  ERROR_PWM_SETUP = 84,
  ERROR_BALANCE_INITIATION = 85,
  ERROR_PEC = 86,
  ERROR_CAN_READ = 87, // TORCH CAN ERROR
  // Mobo specific Error codes
  OVERCURRENT = 88 ,    //Overcurrent fail
  CANTIMEOUT_INVERTER = 89, // CAN Inverter Timeout
  CANTIMEOUT_MODULES = 90,   //Can Torch board Timeout 
  CANERROR = 91,       //CAN bus tried restarting >MAX_RECOVERY_ATTEMPTS times
  IMBALANCE = 92,      //cell imbalance > 0.2 V
  PRECHARGE_FAIL = 93   //Precharge took longer than PRECHARGE_TIMEOUT
};

BMS::BMS()
{
    //get these from storage later
    max_discharge_current = 5;
    max_charge_current = 0; 
    
    BMS_Max_Discharge_Current_ID514.set(max_discharge_current);
    BMS_Max_Charge_Current_ID514.set(max_charge_current);
    ESP_LOGI(TAG, "BMS Initialized");
}

BMS *BMS::Get()
{
    if (instancePtr == nullptr && BMS::mutex)
    {
        if (xSemaphoreTake(BMS::mutex, (TickType_t)10) == pdTRUE)
        {
            instancePtr = new BMS();
            xSemaphoreGive(BMS::mutex);
        }
        else
        {
            ESP_LOGW(TAG, "Mutex couldn't be obtained");
        }
    }
    return instancePtr;
}

BMS::STATE BMS::getPackState()
{
    return (STATE)PackStatus_ID1056.get_int();
}