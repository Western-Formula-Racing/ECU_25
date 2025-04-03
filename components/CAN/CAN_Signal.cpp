#include "CAN_Signal.hpp"


CAN_Signal::CAN_Signal(bool isIntel, uint8_t startBit, uint8_t length, float scale, float offset, int32_t default_value)
: isIntel(isIntel), startBit(startBit), length(length), scale(scale), offset(offset), default_value(default_value)
{

}



uint64_t CAN_Signal::get_raw()
{
    return raw_value;
}

uint64_t CAN_Signal::get_uint64()
{
    return static_cast<uint64_t>((raw_value * scale) + offset);
}
int CAN_Signal::get_int()
{
    if(length <= 8){
        return (((int8_t)raw_value * scale) + offset);
    }
    else if(length <= 16){
        return (((int16_t)raw_value * scale) + offset);
    }
    else{
        return (((int32_t)raw_value * scale) + offset);
    }
    
}
bool CAN_Signal::get_bool()
{
    return static_cast<bool>((raw_value * scale) + offset);
}
float CAN_Signal::get_float()
{
    if(length <= 8){
        return static_cast<float>(((int8_t)raw_value * scale) + offset);
    }
    else if(length <= 16){
        return static_cast<float>(((int16_t)raw_value * scale) + offset);
    }
    else{
        return static_cast<float>(((int32_t)raw_value * scale) + offset);
    }
    
}

void CAN_Signal::set_raw(uint64_t value)
{
    raw_value = value;
}

void CAN_Signal::set(uint64_t value)
{
    raw_value = static_cast<uint64_t>((value - offset) / scale);
}
void CAN_Signal::set(int value)
{
    raw_value = static_cast<uint64_t>((value - offset) / scale);
}
void CAN_Signal::set(bool value)
{
    raw_value = static_cast<uint64_t>(value);
}
void CAN_Signal::set(float value)
{
    raw_value = static_cast<uint64_t>((value - offset) / scale);   
}