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
  adc1.setup(SPI2_HOST,ADC_1_CS);
  adc2.setup(SPI2_HOST,ADC_2_CS);
}

int IO::analogRead(analogInputChannel channel){
  int value;
  if(channel>16 || channel < 0){
    ESP_LOGE(TAG,"ADC Channel out of range!");
    return 0;
  }
  if(channel<9){
    value = adc1.readChannel(channel);
  }
  else if(channel>8){
    value = adc2.readChannel(channel-8);
  }
  return value;
};

double IO::analogReadVoltage(analogInputChannel channel){
  double value;
  if(channel>16 || channel < 0){
    ESP_LOGE(TAG,"ADC Channel out of range!");
    return 0;
  }
  if(channel<9){
    value = adc1.readVoltage(channel);
  }
  else if(channel>8){
    value = adc2.readVoltage(channel-8);
  }
  return value;
}