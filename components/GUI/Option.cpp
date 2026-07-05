#include "Option.h"

template<class T>
char* Option<T>::allocate_buffer() 
{
    size_t buffer_size = this->get_json_size();
    if (this->buffer != nullptr)
    {
        this->buffer = new char[buffer_size];
    }
    memset(this->buffer, 0, buffer_size);
    return this->buffer;
}

template<class T>
Option<T>::Option(const char* key, const T* options, size_t option_count)
    : Recievable<T>(key, options[0]),
      value(options[0]),
      default_value(options[0]),
      options(options),
      option_count(option_count),
      buffer(nullptr) 
{
    if constexpr (std::is_same<T, int>::value)
    {
        this->format_spec = "%d";
    } else if constexpr (std::is_same<T, uint32_t>::value)
    {
        this->format_spec = "%ld";
    } else if constexpr (std::is_same<T, float>::value)
    {
        this->format_spec = "%.10f";
    } else if constexpr (std::is_same<T, double>::value)
    {
        this->format_spec = "%.10lf";
    } else if constexpr (std::is_same<T, char*>::value)
    {
        this->format_spec = "%s";
    }
};

template<class T>
Option<T>::Option(const char* key, const T* options, size_t option_count, int default_idx)
    : Recievable<T>(key, options[default_idx]),
      value(options[default_idx]),
      default_value(options[default_idx]),
      options(options),
      option_count(option_count),
      buffer(nullptr) 
{
    if constexpr (std::is_same<T, int>::value)
    {
        this->format_spec = "%d";
    } else if constexpr (std::is_same<T, uint32_t>::value)
    {
        this->format_spec = "%ld";
    } else if constexpr (std::is_same<T, float>::value)
    {
        this->format_spec = "%.10f";
    } else if constexpr (std::is_same<T, float>::value)
    {
        this->format_spec = "%.10f";
    } else if constexpr (std::is_same<T, double>::value)
    {
        this->format_spec = "%.10lf";
    } else if constexpr (std::is_same<T, char*>::value)
    {
        this->format_spec = "%s";
    }
}

template <class T>
size_t Option<T>::get_json_size()
{   
    size_t total_size = 0;
    for (size_t i = 0; i < option_count; i++) {
        if constexpr (std::is_same<T, char*>::value) {
            total_size += strlen(options[i]);
        } else {
            total_size += 32;
        }

        total_size += 52; // Compensate for json formatting characters around each option
    }

    return total_size + 2; // Compensate for json formatting characters and key
};

template<class T>
void Option<T>::cat_json(char* buffer, size_t buffer_size)
{
    char* options_json = this->allocate_buffer();
    size_t options_json_size = this->get_json_size();
    strcat(options_json, "{");
    
    bool first = true;

    size_t value_buffer_size = 32;
    char value_buffer[value_buffer_size];
    snprintf(value_buffer, value_buffer_size, "\"options\": [");
    strcat(options_json, value_buffer);

    for (size_t idx = 0; idx < option_count; idx++)
    {
        if (!first) {
            strcat(options_json, ",");
        }
        first = false;
        snprintf(value_buffer, value_buffer_size, format_spec, options[idx]);
        strncat(options_json, value_buffer, options_json_size - strlen(buffer) - 1);
    }
    
    if constexpr (std::is_same<T, int>::value || std::is_same<T, uint32_t>::value)
    {
        strncat(options_json, "]},\"type\":\"recievable>option>int\"}", options_json_size - 36);
    } else if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
    {
        strncat(options_json, "]},\"type\":\"recievable>option>float\"}", options_json_size - 38);
    } else
    {
        strncat(options_json, "]},\"type\":\"recievable>option>string\"}", options_json_size - 39);
    }

    strncat(buffer, options_json, buffer_size - strlen(buffer) - 1);
    this->deallocate_buffer();
}

template class Option<int>;
template class Option<uint32_t>;
template class Option<float>;
template class Option<double>;
template class Option<char*>;