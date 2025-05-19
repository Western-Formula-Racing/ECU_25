idf_component_register(SRCS "IO.cpp" "TLA2518.cpp" "FXL6408UMX.cpp" "ICM-20948.cpp" "ds3231.cpp" "Logger.cpp"
                  INCLUDE_DIRS "."
                  REQUIRES "driver" "esp_timer" "sdmmc" "fatfs")