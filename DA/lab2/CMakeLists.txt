cmake_minimum_required(VERSION 3.5)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "-pg")

add_executable(prog prog/prog.cpp)

#lab2 src/main.cpp src/TNode.h src/TRBTree.h src/TNode.cpp
