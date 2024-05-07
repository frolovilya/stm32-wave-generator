cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -O0 -g3")

project(integration-tests)

add_executable(${CMAKE_PROJECT_NAME} main.c dwf/device.c dwf/uart.c dwf/scope.c)

find_library(DWF_LIBRARY dwf)
target_link_libraries(${CMAKE_PROJECT_NAME} ${DWF_LIBRARY})
