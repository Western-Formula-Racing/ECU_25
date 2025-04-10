#include "Recievable.h"

template<class T>
Recievable<T>::Recievable(T init_value)
{
    value = init_value;
}

template <class T>
T Recievable<T>::get_value()
{
    return value;
}

template <class T>
void Recievable<T>::set_value(T val)
{
    value = val;
}