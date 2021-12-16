//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_ABSTRACTNODE_H
#define LAB6_ABSTRACTNODE_H


#include <map>
#include "zmq.hpp"
#include "ChildNodeInfo.h"

class AbstractNode {
protected:
    int id;
    int port;
    std::map<int, ChildNodeInfo> outerNodes;
    zmq::context_t context;
    zmq::socket_t recieverSocket;
};


#endif //LAB6_ABSTRACTNODE_H
