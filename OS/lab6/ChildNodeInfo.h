//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_CHILDNODEINFO_H
#define LAB6_CHILDNODEINFO_H


#include <thread>
#include "MessageTypes.h"

class ChildNodeInfo {
    ChildNodeInfo(pid_t pid, int messageRecieverPort, int childRegisterPort):
    pid(pid), messageRecieverPort(messageRecieverPort),
    childRegisterPort(childRegisterPort)
    {}

    pid_t pid;
    int messageRecieverPort;
    int childRegisterPort;
};


#endif //LAB6_CHILDNODEINFO_H
