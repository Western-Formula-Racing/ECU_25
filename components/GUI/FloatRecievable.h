#ifndef _FLOATRECIEVABLE_
#define _FLOATRECIEVABLE_
#include "Recievable.h"
#include <stdio.h>
#include <cstring>

class FloatRecievable : public Recievable<float>
{
private:
    float default_value;
public:
    // Constructor
    FloatRecievable(float default_value);
    
    // Seializes to json
    char* serialize_to_json(void);
};

#endif