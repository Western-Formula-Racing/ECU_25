#ifndef _Option_
#define _Option_
#include "Recievable.h"
#include <cstring>
#include <stdio.h>
#include <vector>
#include <typeinfo>
#include <type_traits>

#define OPTION_BUFFER_SIZE 300

// While there is a template here only int, float and string are supported
template<class T>
class Option : public Recievable<T>
{
private:
    T value;
    T default_value;
    std::vector<T> options;
protected:
    char* buffer;
    
    char* allocate_buffer() {
        delete[] buffer;
        size_t size = OPTION_BUFFER_SIZE * options.size() + 30;
        buffer = new char[size];
        memset(buffer, 0, size);
        return buffer;
    }
public:
    // Constructor
    /**
     * Constructor for the Option class.
     * @param options A vector of possible values for the option. The first element is used as the default value.
     */
    Option(const std::vector<T> options) 
        : Recievable<T>(options.at(0)),
          value(options.at(0)), 
          default_value(options.at(0)), 
          options(options),
          buffer(nullptr) {};
    
    // Constructor
    Option(const std::vector<T> options, int idx) 
        : Recievable<T>(options.at(idx)),
          value(options.at(idx)), 
          default_value(options.at(idx)), 
          options(options), 
          buffer(nullptr) {};
    
    char* serialize_to_json()
    {
        char* buffer = allocate_buffer();
        strcat(buffer, "{\"value\": {\"value\":");
        
        bool first = true;
    
        size_t value_buffer_size = 30;
        char value_buffer[value_buffer_size];
        memset(value_buffer, 0, value_buffer_size);
        if constexpr (std::is_same<T, int>::value)
        {
            snprintf(value_buffer, value_buffer_size, "%d, \"options\": [", value);
            strcat(buffer, value_buffer);
    
            for (T option : options)
            {
                if (!first) {
                    strcat(buffer, ",");
                }
                first = false;
                snprintf(value_buffer, value_buffer_size, "%d", option);
                strcat(buffer, value_buffer);
            }
    
            strcat(buffer, "]},\"type\":\"recievable>option>int\"}");
        } else if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
        {
            snprintf(value_buffer, value_buffer_size, "%.5f, \"options\": [", value);
            strcat(buffer, value_buffer);
    
            for (T option : options)
            {
                if (!first) {
                    strcat(buffer, ",");
                }
                first = false;
                snprintf(value_buffer, value_buffer_size, "%.5f", option);
                strcat(buffer, value_buffer);
            }
    
            strcat(buffer, "]},\"type\":\"recievable>option>float\"}");
        } else 
        {
            snprintf(value_buffer, value_buffer_size, "\"%s\",", value);
            strcat(buffer, value_buffer);
            strcat(buffer, "\"options\": [");
    
            for (T option : options)
            {
                if (!first) {
                    strcat(buffer, ",");
                }
                first = false;
                snprintf(value_buffer, OPTION_BUFFER_SIZE-1, "\"%s\"", option);
                strcat(buffer, value_buffer);
            }
    
            strcat(buffer, "]},\"type\":\"recievable>option>string\"}");
        } 
        return buffer;
    }
};

#endif