/**************************
* 
* Driver for the FXL6408UMX 8-channel GPIO Expander
* https://www.onsemi.com/pdf/datasheet/fxl6408-d.pdf
*
***************************/ 
#include "driver/i2c_master.h"
#include "PinMap.h"

class FXL6408UMX{
  private:
  i2c_master_bus_handle_t i2c_busHandle;
  i2c_master_dev_handle_t i2c_devHandle;
  uint8_t outputStatus = 0;
  public:
  
  void setup(i2c_master_bus_handle_t busHandle);
  void test();
  void writeLevel(ECU_HSD_PIN channel, bool level);
};