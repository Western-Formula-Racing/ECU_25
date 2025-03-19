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

void IO::Setup(){
  spi_bus_config_t spiConfig = {
    
    .mosi_io_num = MOSI_PIN,
    .miso_io_num = MISO_PIN,
    .sclk_io_num = SPI_CLK_PIN,
    .quadwp_io_num = -1,  // Required for non-quad SPI
    .quadhd_io_num = -1,  // Required for non-quad SPI
    .max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE,
    .flags = SPICOMMON_BUSFLAG_MASTER, 
  };

  spi_device_interface_config_t adc1cfg{
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode =0,
    .clock_speed_hz = 1000000,
    .spics_io_num = ADC_1_CS,
    .queue_size=1,
  };

  spi_device_interface_config_t adc2cfg{
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode =0,
    .clock_speed_hz = 1000000,
    .spics_io_num = ADC_2_CS,
    .queue_size=1,
  };

  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST,&spiConfig,SPI_DMA_CH_AUTO));
  ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST,&adc1cfg,&adc1Handle));
  ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST,&adc2cfg,&adc2Handle));

  adcSetup(adc1Handle);
  adcSetup(adc2Handle);

  uint8_t rx_buffer[2] = {0,0};
  while(1){
    vTaskDelay(pdMS_TO_TICKS(500));

    for(int i = 0;i<7;i++){
      adcRead(adc1Handle,i,rx_buffer);
      printf("ADC1 Channel %d: %X,%X\n",i,rx_buffer[0],rx_buffer[1]);
    }
    for(int i = 0;i<7;i++){
      adcRead(adc2Handle,i,rx_buffer);
      printf("ADC2 Channel %d: %X,%X\n",i,rx_buffer[0],rx_buffer[1]);
    }
  }
}

void io_task(void *pvParameters) {
  IO io;
  io.Setup();
}

void adcSetup(spi_device_handle_t devHandle){

  //Force all channels to be analog inputs
  uint8_t tx_buffer[3] = {ADC_CMD_WRITE,0x1,0b00000100};
  spi_transaction_t t = {};
  t.length = 8*sizeof(tx_buffer);
  t.tx_buffer = tx_buffer;
  spi_device_transmit(devHandle,&t);
 
  
  //uint8_t tx_buffer2[3] = {ADC_CMD_WRITE,0x10,0b00000001};
  //spi_transaction_t t2 = {};
  //t2.length = 8*sizeof(tx_buffer2);
  //t2.tx_buffer = tx_buffer;
  //spi_device_transmit(devHandle,&t2);

  //appends ADC channel to end of ADC reading (mainly for debugging)
  uint8_t tx_buffer3[3] = {ADC_CMD_WRITE,0x2,0b00010000};
  spi_transaction_t t3 = {};
  t3.length = 8*sizeof(tx_buffer3);
  t3.tx_buffer = tx_buffer;
  spi_device_transmit(devHandle,&t3);
}

void adcRead(spi_device_handle_t devHandle, uint8_t channel, uint8_t *rx_buffer){ 
  // ADC Read operations return 12 bit read value, and 4 bit ADC channel number 1xxx
  uint8_t tx_buffer[3] = {ADC_CMD_WRITE,0x11,channel};
  spi_transaction_t t = {};
  t.length = 8*sizeof(tx_buffer);
  t.tx_buffer = tx_buffer;
  t.rxlength = 16;
  t.rx_buffer = rx_buffer;
  spi_transaction_t t2 = {};
  t2.length = 16;
  t2.rx_buffer = rx_buffer;
  spi_device_transmit(devHandle,&t);
  spi_device_transmit(devHandle,&t2);
}