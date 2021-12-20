//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_CHILDNODEINFO_H
#define LAB6_CHILDNODEINFO_H


#include <thread>
#include "MessageTypes.h"

class ChildNodeInfo {

public:
    ChildNodeInfo(pid_t pid, int messageRecieverPort, int childRegisterPort):
            Pid(pid), ReceiverPort(messageRecieverPort),
            RegisterPort(childRegisterPort)
    {}
    pid_t Pid;
    int RegisterPort;

    int ReceiverPort;
};


#endif //LAB6_CHILDNODEINFO_H
