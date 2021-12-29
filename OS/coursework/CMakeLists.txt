cmake_minimum_required(VERSION 3.20)
project(coursework)

set(CMAKE_CXX_STANDARD 20)

add_executable(coursework main.cpp Allocator.h BlockAllocator.h BuddyAllocator.h)
