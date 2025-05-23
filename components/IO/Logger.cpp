#include "Logger.h"
static const char* TAG = "Logger";
char filename[128];
FILE* f;
static QueueHandle_t logQueue = NULL;

void Logger::init()
{
    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    const char mount_point[] = "/sdcard";

    ESP_LOGW(TAG, "Initializing SD card");

    // Create a custom host config
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.flags = SDMMC_HOST_FLAG_4BIT; // or SDMMC_HOST_FLAG_4BIT if you wire full 4 data lines
    host.max_freq_khz = SDMMC_FREQ_DEFAULT;

    // Custom slot config
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 4; // or 4 if you use D1-D3 lines too

    // MANUALLY set pins (Important part!)
    slot_config.clk = GPIO_NUM_11;   // Clock
    slot_config.cmd = GPIO_NUM_12;   // Command
    slot_config.d0  = GPIO_NUM_10;    // Data 0
    // Optional if 4-bit mode:
    slot_config.d1  = GPIO_NUM_9;
    slot_config.d2  = GPIO_NUM_14;
    slot_config.d3  = GPIO_NUM_13;

    // Pullups are still needed
    gpio_set_pull_mode(slot_config.clk, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(slot_config.cmd, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(slot_config.d0,  GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(slot_config.d1,  GPIO_PULLUP_ONLY); // If using
    gpio_set_pull_mode(slot_config.d2,  GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(slot_config.d3,  GPIO_PULLUP_ONLY);

    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount SD card (%s)", esp_err_to_name(ret));
        return;
    }
    ESP_LOGW(TAG, "SD card mounted");
    
    tm time;
    char time_string[32];     
    if (IO::Get()->rtc_handle->getTime(time) == ESP_OK) {
        
        strftime(time_string, sizeof(time_string), "%Y-%m-%d-%H-%M-%S", &time);
        printf("RTC Time: %s\n", time_string);
    } else {
        printf("Failed to read RTC time\n");
    }

    // sprintf(filename, "/sdcard/as1---------------f1.csv");
    sprintf(filename, "/sdcard/%s.csv", time_string);
    // sprintf(filename, "/sdcard/test.csv");
    f = fopen(filename, "w");
    if (f == NULL) {
        ESP_LOGE(TAG,"Failed to open file for writing");
        return;
    }
    fclose(f);
    ESP_LOGI(TAG, "File written successfully");


    logQueue = xQueueCreate(512, sizeof(LogMessage_t));
    xTaskCreatePinnedToCore(logTask, "logTask", 4096, NULL, configMAX_PRIORITIES - 1, nullptr, 0);

}

void Logger::writeLine(LogMessage_t message)
{
    f = fopen(filename, "a");
    if (f == NULL) {
        printf("filename: %s\n", filename);
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    // printf("%lld,%s,%s\n", current_time, message.label, message.message);
    fprintf(f, "%lld,%s,%s\n", message.timestamp, message.label, message.message);
    fclose(f);

}

void Logger::log(LogMessage_t message)
{
    message.timestamp = esp_timer_get_time()/1000;
    if (logQueue != NULL)
    {
        if (xQueueSendToBack(logQueue, &message, 0) != pdTRUE)
        {
            ESP_LOGE(TAG, "failed to send message to logQueue");
        }
    }


}

void Logger::logTask(void *)
{
    printf("logging task started!");
    for(;;)
    {
        if (logQueue != NULL)
        {

            LogMessage_t message;
            f = fopen(filename, "a");
            if (f == NULL) {
                printf("filename: %s\n", filename);
                ESP_LOGE(TAG, "Failed to open file for writing");
                return;
            }
            while (xQueueReceive(logQueue, &message, 0) == pdTRUE)
            {
                if (f == NULL) {
                    printf("filename: %s\n", filename);
                    ESP_LOGE(TAG, "Failed to open file for writing");
                    return;
                }
                // printf("%lld,%s,%s\n", message.timestamp, message.label, message.message);
                fprintf(f, "%lld,%s,%s\n", message.timestamp, message.label, message.message);

            }
            fclose(f);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}