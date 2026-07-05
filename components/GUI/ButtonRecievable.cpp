#include "ButtonRecievable.h"

ButtonRecievable::ButtonRecievable(const char* key, void (*callable)(void))
    : Recievable<void (*)(void)>(key, callable)
{
    this->value = callable;
}

size_t ButtonRecievable::get_json_size()
{
    return 29;
}

void ButtonRecievable::cat_json(char *buffer, size_t buffer_size)
{
    strncat(buffer, "{\"type\":\"recievable>button\"}", buffer_size - strlen(buffer) - 1);
}