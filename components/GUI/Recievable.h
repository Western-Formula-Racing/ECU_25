#ifndef _RECIEVABLE_
#define _RECIEVABLE_
#include <cstring>
#include <stdio.h>
#include <stdexcept>
#include <type_traits>
#include <cstdint>

class BaseRecievable {
public:
    virtual ~BaseRecievable() = default;
    virtual size_t get_json_size() = 0; // Get size of JSON representation of this recievable
    virtual void cat_json(char* buffer, size_t buffer_size) = 0;
};

template <class T>
class Recievable : public BaseRecievable
{
protected:
    T value;
    char* key = new char[32];
    char* buffer;
    virtual char* allocate_buffer() {
        if (buffer == nullptr) {
            buffer = new char[this->get_json_size()];
            memset(buffer, 0, this->get_json_size());
        }
        return buffer;
    }
    virtual void deallocate_buffer() {
        if (buffer) {
            delete[] buffer;
            buffer = nullptr;
        }
    }
public:
    // Constructor
    Recievable(const char* key, T init_value) : value(init_value), buffer(nullptr) 
    {
        strncpy(this->key, key, 31);
        this->key[31] = '\0';
    }

    // Destructor
    ~Recievable() {
        deallocate_buffer();
    }

    // Sets value of recievable
    inline void set_value(T val) {
        value = val;
    }

    // Gets value in recievable 
    inline T get_value() {
        return value;
    }

    // Gets key of recievable
    inline char* get_key() {
        return key;
    }
};

#endif