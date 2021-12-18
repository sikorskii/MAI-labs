//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_ABSTRACTNODE_H
#define LAB6_ABSTRACTNODE_H


#include <map>
#include <unistd.h>
#include "zmq.hpp"
#include "ChildNodeInfo.h"
#include "ZmqUtils.h"

using std::to_string;

class AbstractNode {
public:
    explicit AbstractNode(int id) : Id(id), port(ZmqUtils::PORT_TO_BIND_FROM),
    outerNodes(), context(1), receiverSocket(context, zmq::socket_type::rep){
        occupyPort();
    }

    AbstractNode() {}

    pid_t addChild(int id, int _port) {
        pid_t child = fork();
        if (child == 0) {
            execl("server", to_string(id).c_str(), to_string(Id).c_str(),
                  to_string(port).c_str(), to_string(_port).c_str(), NULL);
        }
        return child;
    }

protected:
    int Id;
    int port;
    std::map<int, ChildNodeInfo> outerNodes;
    zmq::context_t context;
    zmq::socket_t receiverSocket;

    void occupyPort() {
        port = ZmqUtils::occupyPort(receiverSocket);
    }
};


#endif //LAB6_ABSTRACTNODE_H
