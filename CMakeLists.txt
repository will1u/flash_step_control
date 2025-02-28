# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(flash_step_control C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(flash_step_control
    read16.c
)

# Generate PIO headers for the PIO programs
pico_generate_pio_header(flash_step_control ${CMAKE_CURRENT_LIST_DIR}/read16.pio)

# Link the required libraries
target_link_libraries(flash_step_control PRIVATE
    pico_stdlib
    hardware_pio
)

# Enable USB and UART for serial communication
pico_enable_stdio_usb(flash_step_control 1)
pico_enable_stdio_uart(flash_step_control 1)

# Create UF2, bin, hex files
pico_add_extra_outputs(flash_step_control)
