#ifndef _PINMAP_
#define _PINMAP_

#define MOSI_PIN      38  //MOSI pin
#define MISO_PIN      39  //MISO pin
#define SPI_CLK_PIN   40  //SPI Clock pin
#define ADC_1_CS      41  //ADC1 Chip Select pin
#define ADC_2_CS      42  //ADC2 Chip Select pin
#define I2C_SCL_PIN   7
#define I2C_SDA_PIN   17
#define IMU_CS        8

// ECU PINS are all prefixed with ECU_#_ 
// The format is ECU_#_<connector pin numer>_<function><number>
// Functions are A = Analog input, IO = 5v IO, and HSD = 24v high side drivers
// Ex: ECU_1_A1 = connector pin 1 which is Analog pin 1
typedef enum{
    ECU_1_A1,
    ECU_2_A2,
    ECU_3_A3,
    ECU_4_A4,
    ECU_5_A5,
    ECU_6_A6,
    ECU_7_A7,
    ECU_8_A8 ,
    ECU_9_A9 ,
    ECU_13_A10,
    ECU_14_A11,
    ECU_15_A12,
    ECU_16_A13,
    ECU_17_A14,
}ECU_ANALOG_PIN;
typedef enum{
    ECU_TEST = 47,
    ECU_SELECT = 46,
    ECU_10_IO1 = 4,
    ECU_11_IO2 = 5,
    ECU_12_IO3 = 6,
}ECU_IO_PIN;
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