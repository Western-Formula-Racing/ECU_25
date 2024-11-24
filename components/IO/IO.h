#ifndef _IO_
#define _IO_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


class IO{
private:
    //Singleton device class structure
    static IO* instancePtr;
    static SemaphoreHandle_t mutex;
    IO();
public:
    //Deleting the copy constructor and copy reference constructor to prevent copies
    IO(const IO &) = delete;
    IO &operator=(const IO &) = delete;
    IO(IO &&) = delete;
    IO &operator=(IO &&) = delete;
    static IO* Get();

};

#endif