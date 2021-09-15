cmake_minimum_required(VERSION 3.19)
project(kp8 C)

set(CMAKE_C_STANDARD 11)

add_executable(kp8
        main.c list.c list.h)
