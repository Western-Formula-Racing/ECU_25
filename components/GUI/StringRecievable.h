#ifndef _STRINGRECIEVABLE_
#define _STRINGRECIEVABLE_
#include "Recievable.h"
#include <stdio.h>
#include <cstring>

class StringRecievable : public Recievable<char*>
{
private:
    char* default_value;
public:
    // Constructor
    StringRecievable(char* default_value);
    
    // Seializes to json
    char* serialize_to_json(void);
};

#endif