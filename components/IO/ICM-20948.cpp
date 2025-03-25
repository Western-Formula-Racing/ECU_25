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