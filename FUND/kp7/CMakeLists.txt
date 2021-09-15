cmake_minimum_required(VERSION 3.19)
project(_VII C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(_VII
        main.c
        vector.h vector.c)
