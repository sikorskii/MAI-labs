cmake_minimum_required(VERSION 3.20)
project(lab3 C)

set(CMAKE_C_STANDARD 99)

set ( CMAKE_C_FLAGS "-pthread")

add_executable(lab3 src/main.c src/mtxutils.c src/mtxutils.h src/tutils.c src/tutils.h)
