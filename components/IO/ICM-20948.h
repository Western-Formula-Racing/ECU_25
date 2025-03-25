/**************************
* 
* Driver for the icm-20948 9-axis IMU
* https://invensense.tdk.com/products/motion-tracking/9-axis/icm-20948/ 
*
*
* TODO: 
* - Add support for using channels in GPIO mode
* - Add option for continuous reading instead of one-shot
* - i.e. expose more chip functionality
*
***************************/ 

#include "driver/spi_master.h"
#include "PinMap.h"


class ICM20948{
  private:

  spi_device_handle_t imuHandle;
  spi_device_interface_config_t imuConfig;
  
  public:
  void setup(spi_host_device_t spiHost, int cs_pin);
  void selfTest();
  
};