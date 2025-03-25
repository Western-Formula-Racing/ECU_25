#include "ICM-20948.h"

ICM20948::ICM20948(spi_host_device_t spiHost,int cs_pin){
  imuConfig = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = 0,
    .clock_speed_hz = 5000000, //5MHz by default, can go up to 7
    .spics_io_num = cs_pin,
    .queue_size=1,
  };
  
  ESP_ERROR_CHECK(spi_bus_add_device(spiHost,&imuConfig,&imuHandle));
  writeRegister(0, ICM_PWR_MGMT_1, 0x01); // Wake up, auto clock
  writeRegister(2, ICM_ACCEL_CONFIG,accelScale<<1); //configure accelerometer scale
  /* BAD MAGNETOMETER CODE
  //magnetometer init (i2c master)
  writeRegister(0, ICM_USER_CTRL,0X2); //reset i2c master module
  vTaskDelay(pdMS_TO_TICKS(10));
  writeRegister(0, ICM_USER_CTRL,0x20); //enable i2c master mode for magnetometer readings
  writeRegister(3,ICM_I2C_MST_CTRL,0x07); //set i2c clock frequency to 400khz
  vTaskDelay(pdMS_TO_TICKS(10));
  writeRegister(0,ICM_LP_CONFIG,0x40); //set custom i2c data rate
  writeRegister(3,ICM_I2C_MST_ODR_CONFIG,0x03); //set data rate to 136 Hz
  vTaskDelay(pdMS_TO_TICKS(10));
  
  writeMagnetometer(MAG_CTRL3,0x01); //soft reset magnetometer
  vTaskDelay(pdMS_TO_TICKS(100));
  writeMagnetometer(MAG_CTRL2,0x16); //continouous mode 4: 100Hz
  writeRegister(3, ICM_I2C_SLV0_ADDR, 0x8C); // Read address
  writeRegister(3, ICM_I2C_SLV0_REG, 0x10);  // Start from ST1
  writeRegister(3, ICM_I2C_SLV0_CTRL, 0x88); // Enable + read 8 bytes
  */
  
}

void ICM20948::selfTest(){

  uint8_t tx_buffer[3] = {128,0};
  uint8_t rx_buffer[2] = {0,0};
  spi_transaction_t t = {};
  t.length = 8*sizeof(tx_buffer);
  t.tx_buffer = tx_buffer;
  t.rxlength = 8*sizeof(rx_buffer);
  t.rx_buffer = rx_buffer;
  spi_device_transmit(imuHandle,&t);

  printf("Recieved Data: %d, %d",rx_buffer[0],rx_buffer[1]);
}

uint8_t ICM20948::readRegister(uint8_t bank, uint8_t addr){
  //switch register bank
  bank = bank<<4;
  uint8_t tx_buffer0[2] = {0x7F,bank};
  spi_transaction_t t0 = {};
  t0.length = 8*sizeof(tx_buffer0);
  t0.tx_buffer = tx_buffer0;
  spi_device_transmit(imuHandle,&t0);
  //read data
  addr |= 128;
  uint8_t tx_buffer[2] = {addr,0};
  uint8_t rx_buffer[2];
  spi_transaction_t t = {};
  t.length = 8*sizeof(tx_buffer);
  t.tx_buffer = tx_buffer;
  t.rxlength = 8*sizeof(rx_buffer);
  t.rx_buffer = rx_buffer;
  spi_device_transmit(imuHandle,&t);

  return rx_buffer[1]; // for some reason this writes the data in the second byte of rx_buffer
}
void ICM20948::writeRegister(uint8_t bank, uint8_t addr,uint8_t data){
  //switch register bank
  bank = bank<<4;
  uint8_t tx_buffer0[2] = {0x7F,bank};
  spi_transaction_t t0 = {};
  t0.length = 8*sizeof(tx_buffer0);
  t0.tx_buffer = tx_buffer0;
  spi_device_transmit(imuHandle,&t0);
  //write data
  uint8_t tx_buffer[2] = {addr,data};
  spi_transaction_t t = {};
  t.length = 8*sizeof(tx_buffer);
  t.tx_buffer = tx_buffer;
  spi_device_transmit(imuHandle,&t);
}
/* BAD MAGNETOMETER CODE
void ICM20948::writeMagnetometer(uint8_t addr, uint8_t data){
  //set write address of magnetometer
  writeRegister(3,ICM_I2C_SLV0_ADDR,0x0C);
  //set register of magnetometer
  writeRegister(3,ICM_I2C_SLV0_REG,addr);
  //set data to be written
  writeRegister(3,ICM_I2C_SLV0_D0,data);
  //set bit to single data write
  writeRegister(3,ICM_I2C_SLV0_CTRL,0x80|0x01);
}

void ICM20948::readMagnetometer(uint8_t addr,uint8_t len){
    //set write address of magnetometer
    writeRegister(3,ICM_I2C_SLV0_ADDR,0x8C);
    //set first register of magnetometer to be read
    writeRegister(3,ICM_I2C_SLV0_REG,addr);
    //set length of data to be read and read data
    writeRegister(3,ICM_I2C_SLV0_CTRL,0x80|len);
}
*/
double ICM20948::getAccelX(){
  
  uint8_t outH = readRegister(0,ICM_ACCEL_XOUT_H);
  uint8_t outL = readRegister(0,ICM_ACCEL_XOUT_L);

  int16_t sum = outL+(outH<<8);
  double output = sum/(16384.0/(1<<accelScale));
  return output;
}

double ICM20948::getAccelY(){
  
  uint8_t outH = readRegister(0,ICM_ACCEL_YOUT_H);
  uint8_t outL = readRegister(0,ICM_ACCEL_YOUT_L);

  int16_t sum = outL+(outH<<8);
  double output = sum/(16384.0/(1<<accelScale));
  return output;
}

double ICM20948::getAccelZ(){
  
  uint8_t outH = readRegister(0,ICM_ACCEL_ZOUT_H);
  uint8_t outL = readRegister(0,ICM_ACCEL_ZOUT_L);

  int16_t sum = outL+(outH<<8);
  double output = sum/(16384.0/(1<<accelScale));
  return output;
}

double ICM20948::getGyroX(){
  
  uint8_t outH = readRegister(0,ICM_GYRO_XOUT_H);
  uint8_t outL = readRegister(0,ICM_GYRO_XOUT_L);

  int16_t sum = outL+(outH<<8);
  double output = sum/(131.0/(1<<gyroScale));
  return output;
}

double ICM20948::getGyroY(){
  
  uint8_t outH = readRegister(0,ICM_GYRO_YOUT_H);
  uint8_t outL = readRegister(0,ICM_GYRO_YOUT_L);

  int16_t sum = outL+(outH<<8);
  double output = sum/(131.0/(1<<gyroScale));
  return output;
}

double ICM20948::getGyroZ(){
  
  uint8_t outH = readRegister(0,ICM_GYRO_ZOUT_H);
  uint8_t outL = readRegister(0,ICM_GYRO_ZOUT_L);

  int16_t sum = outL+(outH<<8);
  double output = sum/(131.0/(1<<gyroScale));
  return output;
}