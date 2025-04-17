#ifndef _BUTTONRECIEVABLE_
#define _BUTTONRECIEVABLE_
#include <Recievable.h>

class ButtonRecievable : public Recievable<void(*)()>
{
private:
public:
    // Contructor
    ButtonRecievable(void(*)());

    inline void call()
    {
        value();
    }

    // Serializes to json
    char* serialize_to_json(void);
};

#endif