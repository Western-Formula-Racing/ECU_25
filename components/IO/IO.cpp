#include "IO.h"
#include "esp_log.h"
static const char *TAG = "IO"; // Used for ESP_LOGx commands. See ESP-IDF Documentation

IO *IO::instancePtr = nullptr;
SemaphoreHandle_t IO::mutex = xSemaphoreCreateMutex();

IO::IO()
{
    //setup ESP32 native GPIO
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL<<GPIO_NUM_48);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    setupSPI();
    adc1_handle =  new TLA2518(SPI2_HOST, GPIO_NUM_41);
    adc2_handle = new TLA2518(SPI2_HOST, GPIO_NUM_42);
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
<<<<<<< HEAD
  
=======
  adc1.setup(SPI2_HOST,ADC_1_CS);
  adc2.setup(SPI2_HOST,ADC_2_CS);
  imu.setup(SPI2_HOST,IMU_CS);
>>>>>>> c7f861d (established communication with IMU)
}

int IO::analogRead(ECU_ANALOG_PIN pin)
{
    if(pin <= ECU_7_A7){
        return adc1_handle->readChannel(pin);
    }
    else{
        return adc1_handle->readChannel(pin);
    }
}
double IO::analogReadVoltage(ECU_ANALOG_PIN pin)
{
    if(pin <= ECU_7_A7){
        return adc1_handle->readVoltage(pin);
    }
    else{
        return adc1_handle->readVoltage(pin);
    }
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
  hsd_handle = new FXL6408UMX(i2c_handle);
};

void IO::HSDWrite(ECU_HSD_PIN channel, bool level){
    hsd_handle->writeLevel(channel,level);
}