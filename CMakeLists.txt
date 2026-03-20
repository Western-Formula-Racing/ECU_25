# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# Find ETL from whichever PlatformIO environment has installed it
file(GLOB ETL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/.pio/libdeps/*/Embedded Template Library/include")
foreach(ETL_INC ${ETL_INCLUDE_DIRS})
    include_directories("${ETL_INC}")
endforeach()

project(ECU_25)

set(CONFIG_ESP_MAIN_TASK_STACK_SIZE 8192)
