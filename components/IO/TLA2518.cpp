#include "TLA2518.h"

TLA2518::TLA2518(spi_host_device_t spiHost,int cs_pin){

  adcCfg = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = 0,
    .clock_speed_hz = 10000000, //10MHz by default, can go up to 50
    .spics_io_num = cs_pin,
    .queue_size=1,
  };

  ESP_ERROR_CHECK(spi_bus_add_device(spiHost,&adcCfg,&adcHandle));

  //force all channels to be analog inputs
  writeRegister(0x1,4);
}

void TLA2518::writeRegister(uint8_t address, uint8_t value){

  uint8_t tx_buffer[3] = {TLA_CMD_WRITE,address,value};
  spi_transaction_t t = {};
  t.length = 8*sizeof(tx_buffer);
  t.tx_buffer = tx_buffer;
  spi_device_transmit(adcHandle,&t);
};

int TLA2518::readChannel(uint8_t channel){

  uint8_t tx_buffer[3] = {TLA_CMD_WRITE,TLA_CHANNEL_SEL,channel};
  uint8_t rx_buffer[2] = {0,0};
  spi_transaction_t t = {};
  t.length = 8*sizeof(tx_buffer);
  t.tx_buffer = tx_buffer;
  t.rxlength = 8*sizeof(rx_buffer);
  t.rx_buffer = rx_buffer;
  spi_device_transmit(adcHandle,&t);

  return ((int)rx_buffer[0]<<4 | (int)rx_buffer[1]>>4);

};

double TLA2518::readVoltage(uint8_t channel){
  return readChannel(channel)*(5.0/4096.0);
}