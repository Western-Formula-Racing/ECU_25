/**
 * @file CAN_Signal.hpp
 * @brief Defines the individual CAN signals for the messages. 
 */

#ifndef __CANSIGNAL__
#define __CANSIGNAL__
#include <stdio.h>
#include "esp_log.h"
#include <variant>
#include "esp_timer.h"

//If a signal is not recieved for longer then TIMEOUT then we return the default value. 
#define TIMEOUT 2000

/**
 * @class CAN_Signal
 * @brief Represents a single CAN signal, including its position, length, scaling, and value.
 * 
 * @details
 * This class provides methods to encode and decode individual signal values from CAN messages.
 * It supports automatic scaling, endian handling, and timeout logic for stale signals.
 */
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

    /** @brief Sets the signal value using an unsigned integer, applying inverse scaling and offset.
     * 
     * @param value Scaled value to store.
     */
    void set(uint64_t value);           

    /** @brief Sets the signal value using a signed integer, applying inverse scaling and offset.
     * 
     * @param value Scaled value to store.
     */
    void set(int value);

    /** @brief Sets the signal value using a boolean (true = 1, false = 0).
     * 
     * @param value Boolean value to store.
     */
    void set(bool value);               

    /** @brief Sets the signal value using a float, applying scaling and offset.
     * 
     * @param value Scaled value to store.
     */
    void set(float value);              

    /**  @brief Directly sets the raw, unscaled value.
     * 
     * @details Used when decoding CAN messages. Does not apply any scaling or offset.
     * 
     * @param raw_value Raw bitfield value.
     */
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