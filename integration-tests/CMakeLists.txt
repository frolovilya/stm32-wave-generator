cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -O0 -g3")

project(integration-tests)

add_executable(${CMAKE_PROJECT_NAME} main.c device.c uart.c)

find_library(DWF_LIBRARY dwf)
find_path(DWF_INCLUDE_DIR dwf.h)
target_link_libraries(${CMAKE_PROJECT_NAME} ${DWF_LIBRARY})
include_directories(${DWF_INCLUDE_DIR})