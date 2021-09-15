cmake_minimum_required(VERSION 3.19)
project(Kp9 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Kp9
        list.c
        list.h
        main.c)
