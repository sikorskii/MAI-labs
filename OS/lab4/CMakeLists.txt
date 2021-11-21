cmake_minimum_required(VERSION 3.20)
project(lab1)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_executable(lab4.out
        src/main.cpp)#src/example.cpp)
add_executable(child1.out
        src/child1.cpp)

target_link_libraries(lab4.out PRIVATE pthread)
target_link_libraries(lab4.out PRIVATE rt)

target_link_libraries(child1.out PRIVATE pthread)
target_link_libraries(child1.out PRIVATE rt)

