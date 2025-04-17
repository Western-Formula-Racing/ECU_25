#include "ButtonRecievable.h"

ButtonRecievable::ButtonRecievable(void (*callable)()):Recievable(callable)
{
    value = callable;
}

char *ButtonRecievable::serialize_to_json(void)
{
    return nullptr;
}
