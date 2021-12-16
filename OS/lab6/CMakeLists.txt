cmake_minimum_required(VERSION 3.20)
project(lab6)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab6 main.cpp AbstractNode.h SpringBootApplication.h ServerNode.h ChildNodeInfo.h ZmqUtils.h Message.h MessageTypes.h MessageBuilder.h MessageData.h)
target_link_libraries(lab6 zmq)
target_link_libraries(lab6 pthread)