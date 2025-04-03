#ifndef __RECIEVABLE__
#define __RECIEVABLE__

#define RECV_BUFFER_SIZE 200

template <class T>
class Recievable
{
protected:
    T value;
public:
    // Constructors
    Recievable(T init_value);

    // Sets value of recievable
    void set_value(T val);

    // Gets value in recievable
    T get_value();

    // Serializes data to string
    virtual char* serialize_to_json() = 0;
};

#endif