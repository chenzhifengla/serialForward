cmake_minimum_required(VERSION 3.6)
project(serialForward)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES header/serial.h source/main.c)
add_executable(serialForward ${SOURCE_FILES})