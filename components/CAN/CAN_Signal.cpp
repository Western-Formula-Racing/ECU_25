#include "CAN_Signal.hpp"


CAN_Signal::CAN_Signal(bool isIntel, uint8_t startBit, uint8_t length, float scale, float offset, int32_t default_value)
: isIntel(isIntel), startBit(startBit), length(length), scale(scale), offset(offset), default_value(default_value)
{

}


/**
 * @brief Returns the raw, unscaled value for the signal 
 * @return uint64_t 
 */
uint64_t CAN_Signal::get_raw()
{
    //Get the current time on the esp
    int64_t current_time = esp_timer_get_time()/1000;
    
    //If the last time the message was recieved from the signal 
    //is beyond the timeout window -> return the default value instead. 
    if((current_time - last_recieved) >= TIMEOUT){
        return default_value;
    }
    return raw_value;
}

/**
 * @brief Returns the scaled value of the signal as a unsigned, 64bit int
 * 
 * @return uint64_t 
 */
uint64_t CAN_Signal::get_uint64()
{
    return static_cast<uint64_t>((get_raw() * scale) + offset);
}

/**
 * @brief Get the length of the singal value
 * 
 * @details 
 * The raw value is cast based on the signal's bit length:
 * - ≤ 8 bits: cast as int8_t
 * - ≤ 16 bits: cast as int16_t
 * - > 16 bits: cast as int32_t
 * @return int 
 */

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

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool CAN_Signal::get_bool()
{
    return static_cast<bool>((get_raw() * scale) + offset);
}

/**
 * @brief Returns the scaled signal value as a float.
 * 
 * @details 
 * The raw value is cast based on signal length:
 * - ≤ 8 bits: cast as int8_t
 * - ≤ 16 bits: cast as int16_t
 * - > 16 bits: cast as int32_t
 * 
 * @return float The scaled value as a float.
 */
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


/**
 * @brief Updates the raw unscaled value and updates the time that the message was recived. 
 * 
 * @param value 
 */
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