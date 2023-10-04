cmake_minimum_required(VERSION 3.20)
include(pico_sdk_import.cmake)
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_executable(test
src/dht20.c
src/dht20.h
src/main.c
)

pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 1)
pico_add_extra_outputs(test)
target_link_libraries(test pico_stdlib hardware_i2c)
