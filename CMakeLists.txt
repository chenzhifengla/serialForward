cmake_minimum_required(VERSION 3.6)
project(serialForward)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES serial.h main.c serial.c pty.c pty.h)
add_executable(serialForward ${SOURCE_FILES})