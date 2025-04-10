#include "FloatRecievable.h"

FloatRecievable::FloatRecievable(float default_value):Recievable<float>(default_value)
{
    this->default_value = default_value;
}

char* FloatRecievable::serialize_to_json()
{
    char* buffer = new char[RECV_BUFFER_SIZE];

    strncpy(buffer, "{\"value:\"", RECV_BUFFER_SIZE-1);

    char* value_buffer = new char[15];
    snprintf(value_buffer, 15, "%.5f", get_value());

    strncat(buffer, value_buffer, RECV_BUFFER_SIZE-1);
    strncat(buffer, ", type:\"float\"}", RECV_BUFFER_SIZE-1);

    return buffer;
}