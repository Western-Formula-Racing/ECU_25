#include "StringRecievable.h"

StringRecievable::StringRecievable(char* default_value):Recievable<char*>(default_value)
{
    this->default_value = default_value;
    value = default_value;
}

char *StringRecievable::serialize_to_json(void)
{
    char* buffer = allocate_buffer();
    snprintf(buffer, RECV_BUFFER_SIZE-1, "{\"value\":\"%s\", \"type\":\"recievable>string\"}", value);
    return buffer;
}