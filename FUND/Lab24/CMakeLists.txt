cmake_minimum_required(VERSION 3.19)
project(C24 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(C24
        cstack.c
        cstack.h
        ctree24.c
        ctree24.h
        main.c
        utils.c
        utils.h)
