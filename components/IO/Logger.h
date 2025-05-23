#ifndef __LOGGER__
#define __LOGGER__

#include <time.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_log.h"
#include "IO.h"
#include "esp_timer.h"


namespace Logger
{
    enum LogLevel
    {
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR
    };

    typedef struct
    {
        int64_t timestamp;
        char label[32];
        char message[256];
    } LogMessage_t;

    void init();
    void writeLine(LogMessage_t message);
    void log(LogMessage_t message);
    void logTask(void *);
    

}
#endif