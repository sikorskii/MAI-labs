cmake_minimum_required(VERSION 3.20)
project(lab1)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab1
        src/main.cpp src/trapeze.cpp headers/trapeze.h headers/figure.h src/rhombus.cpp headers/rhombus.h src/pentagon.cpp headers/pentagon.h src/point.cpp headers/point.h headers/figures.h)
