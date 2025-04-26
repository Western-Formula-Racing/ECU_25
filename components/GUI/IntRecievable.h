#ifndef _INTRECIEVABLE_
#define _INTRECIEVABLE_
#include "Recievable.h"
#include <stdio.h>
#include <cstring>

class IntRecievable : public Recievable<int>
{
private:
    int default_value;
public:
    // Constructor
    IntRecievable(int default_value);
    
    // Seializes to json
    char* serialize_to_json(void);
};

#endif