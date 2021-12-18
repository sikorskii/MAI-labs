//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_CHILDNODEINFO_H
#define LAB6_CHILDNODEINFO_H


#include <thread>
#include "MessageTypes.h"

class ChildNodeInfo {

    pid_t pid;
    int childRegisterPort;
public:
    int messageRecieverPort;

    ChildNodeInfo(pid_t pid, int messageRecieverPort, int childRegisterPort):
    pid(pid), messageRecieverPort(messageRecieverPort),
    childRegisterPort(childRegisterPort)
    {}
};


#endif //LAB6_CHILDNODEINFO_H
