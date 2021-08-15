cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(pico_sdk_import.cmake)
project(my_project C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)
# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(audio-and-led audio-and-led.c)

pico_enable_stdio_uart(audio-and-led 1)
pico_enable_stdio_usb(audio-and-led 0)
# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(
    audio-and-led
    pico_stdlib
    hardware_dma
    hardware_pio
    hardware_timer
    hardware_clocks
    hardware_pwm
    )

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(audio-and-led)
