cmake_minimum_required(VERSION 3.19)
project(kp6 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(kp6
        list.c
        list.h
        main.c)
