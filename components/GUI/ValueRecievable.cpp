#include "ValueRecievable.h"

template <class T>
size_t ValueRecievable<T>::get_json_size()
{
    return static_cast<size_t>(47);
}

template <class T>
void ValueRecievable<T>::cat_json(char *buffer, size_t buffer_size)
{
    char* value_str = this->allocate_buffer();

    if constexpr (std::is_same_v<T, char*>)
    {
        snprintf(value_str, this->get_json_size() - 1, "{\"value\":\"%s\", \"type\":\"recievable>value\"}", this->value);
    } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, bool>)
    {
        snprintf(value_str, this->get_json_size() - 1, "{\"value\":%d, \"type\":\"recievable>value\"}", this->value);
    } else if constexpr (std::is_same_v<T, uint32_t>)
    {
        snprintf(value_str, this->get_json_size() - 1, "{\"value\":%ld, \"type\":\"recievable>value\"}", this->value);
    } else if constexpr (std::is_same_v<T, float>)
    {
        snprintf(value_str, this->get_json_size() - 1, "{\"value\":%.10f, \"type\":\"recievable>value\"}", this->value);
    } else if constexpr (std::is_same_v<T, double>)
    {
        snprintf(value_str, this->get_json_size() - 1, "{\"value\":%.10lf, \"type\":\"recievable>value\"}", this->value);
    } else 
    {
        snprintf(value_str, this->get_json_size() - 1, "{}", this->value);
    }

    strncat(buffer, value_str, buffer_size - strlen(buffer) - 1);
    this->deallocate_buffer();
}

// Explicit instantiations for Recievables used by the application
template class ValueRecievable<int>;
template class ValueRecievable<bool>;
template class ValueRecievable<uint32_t>;
template class ValueRecievable<float>;
template class ValueRecievable<double>;
template class ValueRecievable<char*>;