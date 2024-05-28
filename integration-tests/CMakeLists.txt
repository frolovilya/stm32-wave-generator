cmake_minimum_required(VERSION 3.22)

set(TARGET_FLAGS "-Wall -Wextra -O0 -g3")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${TARGET_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TARGET_FLAGS}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED SCOPE_CHANNEL)
    set(SCOPE_CHANNEL 1)
endif()
message("Scope channel: " ${SCOPE_CHANNEL})

if(NOT DEFINED DIGITAL_TX_PIN)
    set(DIGITAL_TX_PIN 0)
endif()
message("Digital TX pin: " ${DIGITAL_TX_PIN})

if(NOT DEFINED DIGITAL_RX_PIN)
    set(DIGITAL_RX_PIN 1)
endif()
message("Digital RX pin: " ${DIGITAL_RX_PIN})

project(integration-tests VERSION 0.1 LANGUAGES CXX)

enable_language(C CXX ASM)

add_executable(${CMAKE_PROJECT_NAME} 
    #main.c
    #test_utils.c
    #dwf/device.c 
    #dwf/uart.c 
    #dwf/scope.c
    Main.cpp
    dwf/DwfDeviceHandler.cpp
    dwf/Oscilloscope.cpp
    dwf/UART.cpp
    TestUtils.cpp
    TestSuite.cpp
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    SCOPE_CHANNEL=${SCOPE_CHANNEL}
    DIGITAL_TX_PIN=${DIGITAL_TX_PIN}
    DIGITAL_RX_PIN=${DIGITAL_RX_PIN}
)

find_library(DWF_LIBRARY dwf)
target_link_libraries(${CMAKE_PROJECT_NAME} ${DWF_LIBRARY})
find_path(DWF_HEADER dwf.h)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${DWF_HEADER})