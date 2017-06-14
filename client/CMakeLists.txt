cmake_minimum_required(VERSION 3.6)
project(serialServer)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(SOURCE_FILES serial.h main.c serial.c)
add_executable(serialServer ${SOURCE_FILES})