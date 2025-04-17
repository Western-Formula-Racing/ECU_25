#include "StringRecievable.h"

StringRecievable::StringRecievable(char* default_value):Recievable<char*>(default_value)
{
    this->default_value = default_value;
    value = default_value;
}

char *StringRecievable::serialize_to_json(void)
{
    char buffer[RECV_BUFFER_SIZE];
    memset(buffer, 0, RECV_BUFFER_SIZE);
    snprintf(buffer, RECV_BUFFER_SIZE-1, "{\"value\":\"%s\", \"type\":\"recievable>string\"}", get_value());
    return buffer;
}