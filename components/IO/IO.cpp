#include "IO.h"
#include "esp_log.h"
static const char *TAG = "IO"; // Used for ESP_LOGx commands. See ESP-IDF Documentation
#define ESP_INTR_FLAG_DEFAULT 0

IO* IO::instancePtr = nullptr;
SemaphoreHandle_t IO::mutex = xSemaphoreCreateMutex();
TLA2518* IO::adc1_handle = nullptr;
TLA2518* IO::adc2_handle = nullptr;
volatile uint64_t IO::left_wheel_tick = 0;
volatile uint64_t IO::right_wheel_tick = 0;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    if(gpio_num == ECU_11_IO2){
      IO::left_wheel_tick++;
    }
    else if(gpio_num == ECU_12_IO3){
      IO::right_wheel_tick++;
    }
}

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
    
    io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL<<ECU_TEST) | (1ULL<<ECU_10_IO1) | (1ULL<<ECU_11_IO2) | (1ULL<<ECU_12_IO3) | (1ULL<<ECU_SELECT);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&io_conf);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add((gpio_num_t) ECU_11_IO2, gpio_isr_handler, (void*) ECU_11_IO2);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add((gpio_num_t) ECU_12_IO3, gpio_isr_handler, (void*) ECU_12_IO3);
    
    setupSPI();
    setupI2C();
    adc1_handle =  new TLA2518(SPI2_HOST, GPIO_NUM_41);
    adc2_handle = new TLA2518(SPI2_HOST, GPIO_NUM_42);
    imu_handle = new ICM20948(SPI2_HOST,IMU_CS);
    ESP_LOGI(TAG, "IO Initialized");
    
}

IO*  IO::Get()
{
    if (instancePtr == nullptr && IO::mutex)
    {
        if (xSemaphoreTake(IO::mutex, (TickType_t)10) == pdTRUE)
        {
            IO* temp = new IO();          
            instancePtr = temp;
            xSemaphoreGive(IO::mutex);
        }
        else
        {
            ESP_LOGW(TAG, "Mutex couldn't be obtained");
        }
    } 
    return instancePtr;
}

void IO::setupSPI()  {

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

int IO::analogRead(ECU_ANALOG_PIN pin)
{
    if(pin <= ECU_8_A8){
        return IO::adc1_handle->readChannel(pin);
    }
    else{
        pin = static_cast<ECU_ANALOG_PIN>(static_cast<int>(pin) - 8);
        return IO::adc2_handle->readChannel(pin);
    }
}
float IO::analogReadVoltage(ECU_ANALOG_PIN pin)
{
    if(pin >= 8){
      pin = static_cast<ECU_ANALOG_PIN>(static_cast<int>(pin) - 8);
      float value = IO::adc2_handle->readVoltage(pin);
      return value;
    }
    else{
      float value = IO::adc1_handle->readVoltage(pin);
      return value;
    }
}

void IO::setupI2C(){
  i2c_master_bus_config_t i2c_mst_config = {
    .i2c_port = I2C_NUM_0,
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
  rtc_handle = new DS3231(i2c_handle);
};

void IO::HSDWrite(ECU_HSD_PIN channel, bool level){
    hsd_handle->writeLevel(channel,level);
}


int IO::digitalRead(ECU_IO_PIN pin){
    return gpio_get_level((gpio_num_t)pin);
}

float IO::getAccelX(){
  return imu_handle->getAccelX();
}

float IO::getAccelY(){
  return imu_handle->getAccelY();
}

float IO::getAccelZ(){
  return imu_handle->getAccelZ();
}

float IO::getGyroX(){
  return imu_handle->getGyroX();
}

float IO::getGyroY(){
  return imu_handle->getGyroY();
}

float IO::getGyroZ(){
  return imu_handle->getGyroZ();
}

float IO::getLeftWheelSpeed()
{
  return 0.0f;
}
float IO::getRightWheelSpeed()
{
  return 0.0f;
}