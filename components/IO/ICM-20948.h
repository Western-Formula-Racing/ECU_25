/**************************
* 
* Driver for the icm-20948 9-axis IMU
* https://invensense.tdk.com/products/motion-tracking/9-axis/icm-20948/ 
*
*
* TODO: 
* figure out magnetometer reading, currently does not work
*
***************************/ 

#include "driver/spi_master.h"
#include "PinMap.h"

//User Bank 1

#define ICM_USER_CTRL     0x3
#define ICM_LP_CONFIG     0x5
#define ICM_PWR_MGMT_1    0x6
#define ICM_PWR_MGMT_2    0x7
#define ICM_ACCEL_XOUT_H  0x2D
#define ICM_ACCEL_XOUT_L  0x2E
#define ICM_ACCEL_YOUT_H  0x2F
#define ICM_ACCEL_YOUT_L  0x30
#define ICM_ACCEL_ZOUT_H  0x31
#define ICM_ACCEL_ZOUT_L  0x32
#define ICM_GYRO_XOUT_H   0x33
#define ICM_GYRO_XOUT_L   0x34
#define ICM_GYRO_YOUT_H   0x35
#define ICM_GYRO_YOUT_L   0x36
#define ICM_GYRO_ZOUT_H   0x37
#define ICM_GYRO_ZOUT_L   0x38

#define ICM_EXT_SLV_SENS_DATA_00 0x3B

//User bank 2
#define ICM_ACCEL_CONFIG  0x14
#define ICM_GYRO_CONFIG   0x1

//user bank 3
#define ICM_I2C_MST_ODR_CONFIG  0x0
#define ICM_I2C_MST_CTRL    0x1
#define ICM_I2C_SLV0_ADDR   0x3
#define ICM_I2C_SLV0_REG    0x4
#define ICM_I2C_SLV0_CTRL   0x5
#define ICM_I2C_SLV0_D0     0x6

//magnetometer registers
#define MAG_HXL   0x11
#define MAG_CTRL2 0x31
#define MAG_CTRL3 0x32


class ICM20948{
  private:

  spi_device_handle_t imuHandle;
  spi_device_interface_config_t imuConfig;
  
  public:

  uint8_t accelScale = 1; // (0,1,2,3) => +-(2g,4g,8g,16g)
  uint8_t gyroScale = 0;  // (0,1,2,3) => +-(250,500,1000,2000)dps

  ICM20948(spi_host_device_t spiHost, int cs_pin);
  void selfTest();
  uint8_t readRegister(uint8_t bank, uint8_t addr);
  void writeRegister(uint8_t bank, uint8_t addr,uint8_t data);

  //void writeMagnetometer(uint8_t addr, uint8_t data);
  //void readMagnetometer(uint8_t addr,uint8_t len);

  double getAccelX();
  double getAccelY();
  double getAccelZ();
  double getGyroX();
  double getGyroY();
  double getGyroZ();
};