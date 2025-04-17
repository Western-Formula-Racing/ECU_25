#include "Option.h"

template<class T>
Option<T>::Option(T* options, T default_option):Recievable<T>(default_option)
{
    default_value = default_option;
    value = default_option;

    this->options = options;
}

template <class T>
inline T Option<T>::get_value()
{
    return value;
}

template <class T>
inline void Option<T>::set_value(T val)
{
    if (exists(value))
    {
        value = val;
    }
}

template <class T>
inline bool Option<T>::exists(T val)
{
    bool exists = false;
    for (T option : options) 
    {
        if (val == option) 
        {
            exists = true;
            break;
        }
    }

    return false;
}

template <class T>
char *Option<T>::serialize_to_json()
{
    char* buffer = new char[OPTION_BUFFER_SIZE];
    memset(buffer, 0, OPTION_BUFFER_SIZE);
    strcpy(buffer, "{\"value\": {\"value\":");
    
    bool first = true;

    size_t value_buffer_size = 20;
    char value_buffer[value_buffer_size];
    memset(value_buffer, 0, value_buffer_size);
    if (typeid(T) == typeid(int))
    {
        snprintf(value_buffer, value_buffer_size, "%d,", get_value());
        strcat(buffer, value_buffer);
        strcat(buffer, "\"options\": [");

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
    } else if (typeid(T) == typeid(float))
    {
        snprintf(value_buffer, value_buffer_size, "%.5f,", get_value());
        strcat(buffer, value_buffer);
        strcat(buffer, "\"options\": [");

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
        snprintf(value_buffer, value_buffer_size, "\"%s\",", get_value());
        strcat(buffer, value_buffer);
        strcat(buffer, "\"options\": [");

        for (T option : options)
        {
            if (!first) {
                strcat(buffer, ",");
            }
            first = false;
            snprintf(value_buffer, OPTION_BUFFER_SIZE-1, "\"%s\"", option);
            strcat(buffer, option);
        }

        strcat(buffer, "]},\"type\":\"recievable>option>string\"}");
    } 

    return buffer;
}
