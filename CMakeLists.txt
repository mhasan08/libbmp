cmake_minimum_required(VERSION 3.8)
project(bmp)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c bmp/bmp.c bmp/bmp.h)
add_executable(bmp ${SOURCE_FILES})