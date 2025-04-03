#include "StringRecievable.h"

StringRecievable::StringRecievable(char* default_value):Recievable<char*>(default_value)
{
    this->default_value = default_value;
    value = default_value;
}

char *StringRecievable::serialize_to_json(void)
{
    char* buffer = new char[RECV_BUFFER_SIZE];

    strncpy(buffer, "{\"value:\"", RECV_BUFFER_SIZE-1);
    strncat(buffer, get_value(), RECV_BUFFER_SIZE-1);
    strncat(buffer, ", type:\"string\"}", RECV_BUFFER_SIZE-1);

    return buffer;
}