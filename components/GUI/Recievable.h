#ifndef __RECIEVABLE__
#define __RECIEVABLE__

#define RECV_BUFFER_SIZE 200

template <class T>
class Recievable
{
protected:
    T value;
public:
    // Constructor
    Recievable(T init_value) {
        value = init_value;
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