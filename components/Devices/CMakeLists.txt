idf_component_register(SRCS "RearECU.cpp" "BMS.cpp" "Inverter.cpp" "Pedals.cpp" "Devices.cpp" "pwm.cpp" "DRS.cpp"
                  INCLUDE_DIRS "."
                  REQUIRES CAN IO Sensors)