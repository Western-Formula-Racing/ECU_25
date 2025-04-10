#include "IntRecievable.h"

IntRecievable::IntRecievable(int default_value):Recievable<int>(default_value)
{
    this->default_value = default_value;
    value = default_value;
}

char *IntRecievable::serialize_to_json(void)
{
    char* buffer = new char[RECV_BUFFER_SIZE];

    strncpy(buffer, "{\"value:\"", RECV_BUFFER_SIZE-1);

    char* value_buffer = new char[15];
    snprintf(value_buffer, 15, "%d", get_value());

    strncat(buffer, value_buffer, RECV_BUFFER_SIZE-1);
    strncat(buffer, ", type:\"int\"}", RECV_BUFFER_SIZE-1);

    return buffer;
}