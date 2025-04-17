#include "IntRecievable.h"

IntRecievable::IntRecievable(int default_value):Recievable<int>(default_value)
{
    this->default_value = default_value;
    value = default_value;
}

char *IntRecievable::serialize_to_json(void)
{
    char buffer[RECV_BUFFER_SIZE];
    memset(buffer, 0, RECV_BUFFER_SIZE);
    snprintf(buffer, RECV_BUFFER_SIZE-1, "{\"value\":%d, \"type\":\"recievable>int\"}", get_value());
    return buffer;
}