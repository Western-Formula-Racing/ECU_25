#include "CAN_Signal.hpp"


CAN_Signal::CAN_Signal(bool isIntel, uint8_t startBit, uint8_t length, float scale, float offset, int32_t default_value)
: isIntel(isIntel), startBit(startBit), length(length), scale(scale), offset(offset), default_value(default_value)
{

}



uint64_t CAN_Signal::get_raw()
{
    int64_t current_time = esp_timer_get_time()/1000;
    if((current_time - last_recieved) >= TIMEOUT){
        return default_value;
    }
    return raw_value;
}

uint64_t CAN_Signal::get_uint64()
{
    return static_cast<uint64_t>((get_raw() * scale) + offset);
}
int CAN_Signal::get_int()
{
    if(length <= 8){
        return (((int8_t)get_raw() * scale) + offset);
    }
    else if(length <= 16){
        return (((int16_t)get_raw() * scale) + offset);
    }
    else{
        return (((int32_t)get_raw() * scale) + offset);
    }
    
}
bool CAN_Signal::get_bool()
{
    return static_cast<bool>((get_raw() * scale) + offset);
}
float CAN_Signal::get_float()
{
    if(length <= 8){
        return static_cast<float>(((int8_t)get_raw() * scale) + offset);
    }
    else if(length <= 16){
        return static_cast<float>(((int16_t)get_raw() * scale) + offset);
    }
    else{
        return static_cast<float>(((int32_t)get_raw() * scale) + offset);
    }
    
}

void CAN_Signal::set_raw(uint64_t value)
{
    last_recieved = esp_timer_get_time()/1000;
    raw_value = value;
}

void CAN_Signal::set(uint64_t value)
{
    last_recieved = esp_timer_get_time()/1000;
    raw_value = static_cast<uint64_t>((value - offset) / scale);
}
void CAN_Signal::set(int value)
{
    last_recieved = esp_timer_get_time()/1000;
    raw_value = static_cast<uint64_t>((value - offset) / scale);
}
void CAN_Signal::set(bool value)
{
    last_recieved = esp_timer_get_time()/1000;
    raw_value = static_cast<uint64_t>(value);
}
void CAN_Signal::set(float value)
{
    last_recieved = esp_timer_get_time()/1000;
    raw_value = static_cast<uint64_t>((value - offset) / scale);   
}