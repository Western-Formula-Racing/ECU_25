#ifndef _BUTTONRECIEVABLE_
#define _BUTTONRECIEVABLE_
#include <cstring>
#include "Recievable.h"

class ButtonRecievable : public Recievable<void (*)(void)>
{
private:
public:
    // Constructor
    ButtonRecievable(const char* key, void (*callable)(void));

    inline void call()
    {
        value();
    }

    size_t get_json_size() override;

    void cat_json(char* buffer, size_t buffer_size) override;
};

#endif