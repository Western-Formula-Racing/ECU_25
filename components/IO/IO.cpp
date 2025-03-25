#include "IO.h"
#include "esp_log.h"
static const char *TAG = "IO"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

IO *IO::instancePtr = nullptr;
SemaphoreHandle_t IO::mutex = xSemaphoreCreateMutex();

IO::IO()
{
    ESP_LOGI(TAG, "IO Initialized");
}

IO *IO::Get()
{
    if (instancePtr == nullptr && IO::mutex)
    {
        if (xSemaphoreTake(IO::mutex, (TickType_t)10) == pdTRUE)
        {
            instancePtr = new IO();
            xSemaphoreGive(IO::mutex);
        }
        else
        {
            ESP_LOGW(TAG, "Mutex couldn't be obtained");
        }
    }
    return instancePtr;
}

void IO::setupSPI(){

  spi_bus_config_t spiConfig = {
    
    .mosi_io_num = MOSI_PIN,
    .miso_io_num = MISO_PIN,
    .sclk_io_num = SPI_CLK_PIN,
    .quadwp_io_num = -1,  // Required for non-quad SPI
    .quadhd_io_num = -1,  // Required for non-quad SPI
    .max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE,
    .flags = SPICOMMON_BUSFLAG_MASTER, 
  };

  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST,&spiConfig,SPI_DMA_CH_AUTO));
  
}

void IO::setupI2C(){
  i2c_master_bus_config_t i2c_mst_config = {
    .i2c_port = -1,
    .sda_io_num = (gpio_num_t)I2C_SDA_PIN,
    .scl_io_num = (gpio_num_t)I2C_SCL_PIN,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7,
    .flags = {
      .enable_internal_pullup = true
    }
  };

  ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &i2c_handle));
  HSD.setup(i2c_handle);
};

void IO::HSDWrite(ECU_HSD_PIN channel, bool level){
  HSD.writeLevel(channel,level);
}