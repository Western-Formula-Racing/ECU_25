#include "Option.h"

template<class T>
Option<T>::Option(T* options, T default_option):Recievable<T>(default_option)
{
    default_value = default_option;
    value = default_option;

    this->options = options;
}

template <class T>
T Option<T>::get_value()
{
    return value;
}

template <class T>
void Option<T>::set_value(T val)
{
    if (exists(value))
    {
        value = val;
    }
}

template <class T>
bool Option<T>::exists(T val)
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

    strncpy(buffer, "{\"value\":", OPTION_BUFFER_SIZE-1);
    
    size_t value_buffer_size = 20;
    char* value_buffer = new char[value_buffer_size];
    if (typeid(T) == typeid(int))
    {
        snprintf(value_buffer, value_buffer_size, "%d", get_value());
        strncat(buffer, value_buffer, OPTION_BUFFER_SIZE - 1);
        strncat(buffer, "\"type\":\"recievable>option>int\"}", OPTION_BUFFER_SIZE - 1);
    } else if (typeid(T) == typeid(float))
    {
        snprintf(value_buffer, value_buffer_size, "%.5f", get_value());
        strncat(buffer, value_buffer, OPTION_BUFFER_SIZE - 1);
        strncat(buffer, "\"type\":\"recievable>option>float\"}", OPTION_BUFFER_SIZE - 1);
    } else 
    {
        snprintf(value_buffer, value_buffer_size, "%s", get_value());
        strncat(buffer, value_buffer, OPTION_BUFFER_SIZE - 1);
        strncat(buffer, "\"type\":\"recievable>option>string\"}", OPTION_BUFFER_SIZE - 1);
    } 

    return buffer;
}
