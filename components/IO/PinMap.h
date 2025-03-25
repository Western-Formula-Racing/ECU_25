#ifndef _PINMAP_
#define _PINMAP_

#define MOSI_PIN      38  //MOSI pin
#define MISO_PIN      39  //MISO pin
#define SPI_CLK_PIN   40  //SPI Clock pin
#define ADC_1_CS      41  //ADC1 Chip Select pin
#define ADC_2_CS      42  //ADC2 Chip Select pin
#define I2C_SCL_PIN   7
#define I2C_SDA_PIN   17

typedef enum{
  ECU_37_HSD1 = 7,
  ECU_38_HSD2 = 6,
  ECU_39_HSD3 = 5,
  ECU_40_HSD4 = 4,
  ECU_41_HSD5 = 0,
  ECU_48_HSD6 = 3,
  ECU_49_HSD7 = 2,
  ECU_50_HSD8 = 1,


}ECU_HSD_PIN;

#endif