idf_component_register(SRCS "CAN.cpp" "CAN_Signal.cpp"
                  INCLUDE_DIRS "."
                  REQUIRES "driver" "esp_timer")