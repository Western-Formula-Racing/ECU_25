#ifndef __CANSIGNAL__
#define __CANSIGNAL__
#include <stdio.h>
#include "esp_log.h"
#include <variant>
#include "esp_timer.h"

#define TIMEOUT 2000

class CAN_Signal
{
public:
    CAN_Signal(bool isIntel, uint8_t startBit, uint8_t length, 
    float scale = 1.0f, float offset = 0.0f,
    int32_t default_value = 0);

    uint64_t get_raw();
    uint64_t get_uint64();
    int get_int();
    bool get_bool();
    float get_float();

    void set(uint64_t value);
    void set(int value);
    void set(bool value);
    void set(float value);
    void set_raw(uint64_t raw_value);

    
    bool isIntel;
    uint8_t startBit;
    uint8_t length;
    float scale;
    float offset;
private:
    uint64_t raw_value;
    uint64_t last_recieved;
    uint64_t default_value;

};
#endif