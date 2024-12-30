#ifndef __CANSIGNAL__
#define __CANSIGNAL__
#include <stdio.h>
#include "esp_log.h"
#include <variant>

class CAN_Signal
{
public:
    CAN_Signal(bool isIntel, uint8_t startBit, uint8_t length, int32_t scale, int32_t offset);
    uint64_t get_uint64();
    int64_t get_int64;
    bool get_bool();
    float get_float();

    void set(uint64_t value);
    void set(int64_t value);
    void set(bool value);
    void set(float value);

    void set_raw(uint64_t raw_value);


private:
    uint64_t raw_value;
    bool isIntel;
    uint8_t startBit;
    uint8_t length;
    int32_t scale;
    int32_t offset;

};
#endif