#ifndef _VALUERECIEVABLE_
#define _VALUERECIEVABLE_
#include <cstring>
#include <type_traits>
#include "Recievable.h"

template <class T>
class ValueRecievable : public Recievable<T>
{
public:
    ValueRecievable(const char* key, T default_value) : Recievable<T>(key, default_value) {}

    size_t get_json_size() override;
    void cat_json(char* buffer, size_t buffer_size) override;
};

#endif