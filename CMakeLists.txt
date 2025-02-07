# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# You gotta do this for every library you add btw
# Add the ETL library from PlatformIO's libdeps folder
set(ETL_LIB_PATH "${CMAKE_SOURCE_DIR}/.pio/libdeps/esp32-s3-devkitm-1/Embedded Template Library/include")
# Add ETL to the global include directories
include_directories("${ETL_LIB_PATH}")

project(ECU_25)
