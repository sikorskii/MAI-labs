cmake_minimum_required(VERSION 3.20)
project(lab6)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab6 main.cpp AbstractNode.h ChildNodeInfo.h ZmqUtils.h Message.h MessageTypes.h MessageBuilder.h MessageData.h SpringBootApplication.h)
target_link_libraries(lab6 zmq)
target_link_libraries(lab6 pthread)

add_executable(server AbstractNode.h ChildNodeInfo.h ZmqUtils.h Message.h MessageTypes.h MessageBuilder.h MessageData.h SpringBootApplication.h server.cpp)

target_link_libraries(server zmq)
target_link_libraries(server pthread)
