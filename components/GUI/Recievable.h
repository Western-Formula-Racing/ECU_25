#ifndef _RECIEVABLE_
#define _RECIEVABLE_
#include <cstring>
#include <stdio.h>
#define RECV_BUFFER_SIZE 200

template <class T>
class Recievable
{
protected:
    T value;
    char* buffer = nullptr;
    char* allocate_buffer() {
        if (buffer) {
            delete[] buffer;
        }
        buffer = new char[RECV_BUFFER_SIZE];
        memset(buffer, 0, RECV_BUFFER_SIZE);
        return buffer;
    }
public:
    // Constructor
    Recievable(T init_value) : value(init_value), buffer(nullptr) {}

    // Destructor
    virtual ~Recievable() {
        if (buffer != nullptr) {
            delete[] buffer;
            buffer = nullptr;
        }
    }

    // Sets value of recievable
    inline void set_value(T val) {
        value = val;
    }

    // Gets value in recievable 
    inline T get_value() {
        return value;
    }

    // Serializes data to string
    virtual char* serialize_to_json() = 0;
};

#endif