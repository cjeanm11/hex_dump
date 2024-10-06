cmake_minimum_required(VERSION 3.29)
project(hex_dump C)

set(CMAKE_C_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(hex_dump src/main.c
        include/hex_dump.h
        src/hex_dump.c)
