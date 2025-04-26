#include "FloatRecievable.h"

FloatRecievable::FloatRecievable(float default_value):Recievable<float>(default_value)
{
    this->default_value = default_value;
}

char* FloatRecievable::serialize_to_json()
{
    char* buffer = allocate_buffer();
    snprintf(buffer, RECV_BUFFER_SIZE-1, "{\"value\":%.5f, \"type\":\"recievable>float\"}", value);
    return buffer;
}