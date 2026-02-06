cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(pico_acc
  pico_acc.c
)

add_library(adxl345 reg.c adxl345.c)

pico_enable_stdio_usb(pico_acc 1)
pico_enable_stdio_uart(pico_acc 1)

pico_add_extra_outputs(pico_acc)

target_link_libraries(pico_acc pico_stdlib pico_i2c_slave adxl345)
target_link_libraries(adxl345 pico_stdlib pico_i2c_slave)
