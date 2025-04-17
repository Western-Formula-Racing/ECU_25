/**************************
* 
* Driver for the TLA2518 8-channel 12-bit ADC
* https://www.ti.com/product/TLA2518
*
*
* TODO: 
* - Add support for using channels in GPIO mode
* - Add option for continuous reading instead of one-shot
* - i.e. expose more chip functionality
*
***************************/ 

#ifndef TLA2518_H
#define TLA2518_H

#include "driver/spi_master.h"

// Commands
#define TLA_CMD_READ      0b00010000
#define TLA_CMD_WRITE     0b00001000
#define TLA_CMD_SET_BIT   0b00011000
#define TLA_CMD_CLEAR_BIT 0b00100000

// Registers
#define TLA_SYSTEM_STATUS   0x0
#define TLA_GENERAL_CFG     0x1
#define TLA_DATA_CFG        0x2
#define TLA_OSR_CFG         0x3
#define TLA_OPMODE_CFG      0x4
#define TLA_PIN_CFG         0x5
#define TLA_GPIO_CFG        0x7
#define TLA_GPIO_DRIVE_CFG  0x9
#define TLA_GPO_VALUE       0xB
#define TLA_GPI_VALUE       0xD
#define TLA_SEQUENCE_CFG    0x10
#define TLA_CHANNEL_SEL     0x11
#define TLA_AUTO_SEQ_CH_SEL 0x12

class TLA2518{
  
  private:

  spi_device_handle_t adcHandle;
  spi_device_interface_config_t adcCfg;

  public:
  
  TLA2518(spi_host_device_t spiHost, int cs_pin); 
  int readChannel(uint8_t channel);
  double readVoltage(uint8_t channel);
  void writeRegister(uint8_t address, uint8_t value);
};

#endif