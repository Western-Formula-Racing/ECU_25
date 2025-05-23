#include "FXL6408UMX.h"

FXL6408UMX::FXL6408UMX(i2c_master_bus_handle_t busHandle){
  i2c_busHandle = busHandle;
  i2c_device_config_t dev_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = 0b1000011,
    .scl_speed_hz = 50000,
  };
  ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_busHandle, &dev_cfg, &i2c_devHandle));

  //configure all pins as outputs
  uint8_t tx[2] = {0x3,0xFF};
  i2c_master_transmit(i2c_devHandle,tx,sizeof(tx),-1);
  //remove high-z state
  uint8_t tx2[2] = {0x7,0x0};
  i2c_master_transmit(i2c_devHandle,tx2,sizeof(tx2),-1);
}

void FXL6408UMX::test(){
  uint8_t tx = 0x03;
  uint8_t rx;
  i2c_master_transmit_receive(i2c_devHandle,&tx,sizeof(tx),&rx,sizeof(rx),-1);
  printf("Pin config: %d\n",rx);
  tx = 0x05;
  i2c_master_transmit_receive(i2c_devHandle,&tx,sizeof(tx),&rx,sizeof(rx),-1);
  printf("Output Status: %d\n",rx);
}

void FXL6408UMX::writeLevel(int channel, bool level){
  if(level==0){
    outputStatus = outputStatus & ~(1<<channel);
  }
  else if(level==1){
    outputStatus = outputStatus | (1<<channel);
  }
  uint8_t tx[2] = {0x5,outputStatus};
  i2c_master_transmit(i2c_devHandle,tx,sizeof(tx),-1);
}