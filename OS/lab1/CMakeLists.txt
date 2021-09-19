cmake_minimum_required(VERSION 3.20)
project(lab1)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab1.out
        src/main.cpp)#src/example.cpp)
add_executable(child1.out
        src/child1.cpp)
add_executable(child2.out
        src/child2.cpp)
