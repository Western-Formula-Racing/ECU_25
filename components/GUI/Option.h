#ifndef _Option_
#define  _Option_
#include "Recievable.h"
#include <cstring>
#include <stdio.h>
#include <vector>
#include <typeinfo>

#define OPTION_BUFFER_SIZE 300

// While there is a template here only int, float and string are supported
template<class T>
class Option : Recievable<T>
{
private:
    T value;
    T default_value;
    std::vector<T> options;
public:
    // Constructor
    Option(T* options, T default_option);

    // Retrieves value
    T get_value();
    
    // Sets the value
    inline void set_value(T value);

    // Checks if option exists 
    inline bool exists(T value);

    // Serializes data to string
    char* serialize_to_json();
};

#endif