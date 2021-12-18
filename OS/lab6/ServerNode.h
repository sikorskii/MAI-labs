//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_SERVERNODE_H
#define LAB6_SERVERNODE_H


#include "SpringBootApplication.h"
#include "AbstractNode.h"
#include "Message.h"

class ServerNode : public AbstractNode{
public:
    ServerNode(int id, int _parentId, int _parentPort, int _registerPort) :
        AbstractNode(id), parentId(_parentId), parentPort(_parentPort), parentRegPort(_registerPort),
        registerSocket(context, zmq::socket_type::rep) {
        occupyPort();
        registerPort = ZmqUtils::occupyPort(registerSocket);
    }

    void run() {
        std::thread([this]() {registrator();}).detach();
        while (true) {
            Message messageOut;
            Message messageIn;
            messageIn.receiveMessage(receiverSocket);
            switch (messageIn.messageType) {
                case MessageTypes::CREATE_REQUEST:
                    messageOut = createProcessor(messageIn);
                    messageOut.sendMessage(receiverSocket);
                    break;
                case MessageTypes::EXEC_REQUEST:
                    messageOut = execProcessor(messageIn);
                    messageOut.sendMessage(receiverSocket);
                    break;
                case MessageTypes::HEARTBIT_REQUEST:
                    //messageOut = heartbitProcessor(messageIn);
                    messageOut.sendMessage(receiverSocket);
                    break;
                case MessageTypes::QUIT:
                    messageOut = quitProcessor(messageIn);
                    messageOut.sendMessage(receiverSocket);
                    exit(1);
                default:
                    break;
            }
        }
    }
private:
    int parentId;
    int parentPort;
    int parentRegPort;
    zmq::socket_t registerSocket;
    int registerPort;
    std::mutex creationalBlock;
    std::condition_variable creationalCondition;
    bool created;

    void registrator() {
        Message messageIn;
        Message messageOut;
        while (true) {
            messageIn.receiveMessage(registerSocket);
            if (messageIn.messageType == MessageTypes::QUIT) {
                messageIn.sendMessage(registerSocket);
                break;
            }
            messageOut = registerProcessor(messageIn);
            ready();
            messageOut.sendMessage(registerSocket);
        }
    }

    void ready() {
        std::unique_lock<std::mutex> lock(creationalBlock);
        created = true;
        creationalCondition.notify_one();
    }

    void* createRelation(pid_t pid, int receiverPort, int registerPort) {
        auto mem = new char[sizeof(pid) + 2 * sizeof(int)];
        auto memPid = (pid_t*)(mem);
        auto memReceiverPort = (int*)(mem + sizeof(pid_t));
        auto memRegisterPort = (int*)(mem + sizeof(pid_t) + sizeof(int));
        *memPid = pid;
        *memReceiverPort = receiverPort;
        *memRegisterPort = registerPort;
        return (void*)mem;
    }

    void selfRelation(int _registerPort) {
        zmq::socket_t sender(context, zmq::socket_type::req);
        sender.connect(ZmqUtils::getOutputAddress(_registerPort));
        void* relationBody = createRelation(getpid(), port, registerPort);
        Message message(MessageTypes::RELATE_REQUEST, Id, parentId, 2 * sizeof(int) + sizeof(pid_t), relationBody);
        message.sendMessage(sender);
        message.receiveMessage(sender);
        sender.disconnect(ZmqUtils::getOutputAddress(_registerPort));
        delete [](char*)relationBody;
        relationBody = nullptr;
    }

    Message registerProcessor(Message& message) {
        auto messageBody = (char*)message.body;
        pid_t pid = *(pid_t*)(messageBody);
        int receiverPort = *(int*)(messageBody + sizeof(pid_t));
        int registerPort = *(int*)(messageBody + sizeof(pid_t) + sizeof(int));
        outerNodes.emplace(message.senderId, ChildNodeInfo(pid, receiverPort, registerPort));
        return Message(MessageTypes::RELATE_RESULT, Id, message.senderId);
    }

    Message createProcessor(Message& message) {
        int id = ((int*)message.body)[0];
        int parId = ((int*)message.body)[1];
        int sndId = message.senderId;
        if (parId == Id) {
            pid_t pid = addChild(id, registerPort);
            std::unique_lock<std::mutex> lock(creationalBlock);
            created = false;
            while (!created) {
                creationalCondition.wait(lock);
            }
            return {MessageTypes::CREATE_RESULT, Id, parentId, sizeof(pid), &pid};
        }
        else if (outerNodes.empty()) {
            return {MessageTypes::CREATE_FAIL, Id, parentId};
        }
        int pid = -1;
        for (auto server: outerNodes) {
            zmq::socket_t sender(context, zmq::socket_type::req);
            sender.connect(ZmqUtils::getOutputAddress(server.second.messageRecieverPort));
            int data[] = {id, parId};
            Message request(MessageTypes::RELATE_REQUEST, Id, server.first, sizeof(data), (void*)data);
            Message result;
            request.sendMessage(sender);
            result.receiveMessage(sender);
            if (result.messageType == MessageTypes::CREATE_RESULT) {
                pid = *(int*)result.body;
                sender.disconnect(ZmqUtils::getOutputAddress(server.second.messageRecieverPort));
                break;
            }
            sender.disconnect(ZmqUtils::getOutputAddress(server.second.messageRecieverPort));
        }
        if (pid == -1) {
            return {MessageTypes::CREATE_FAIL, Id, sndId, sizeof(pid), &pid};
        }
        return {MessageTypes::CREATE_RESULT, Id, sndId, sizeof(pid), &pid};
    }

    Message quitProcessor(Message& message) {
        Message messageQuit(MessageTypes::QUIT, Id, Id);
        for (auto server: outerNodes) {
            zmq::socket_t request(context, zmq::socket_type::req);
            request.connect(ZmqUtils::getOutputAddress(server.second.messageRecieverPort));
            messageQuit.sendMessage(request);
            Message messageResult;
            messageResult.receiveMessage(request);
            request.disconnect(ZmqUtils::getOutputAddress(server.second.messageRecieverPort));
        }
        zmq::socket_t request(context, zmq::socket_type::req);
        request.connect(ZmqUtils::getOutputAddress(registerPort));
        messageQuit.sendMessage(request);
        messageQuit.receiveMessage(request);
        request.disconnect(ZmqUtils::getOutputAddress(registerPort));
        return messageQuit;
    }

};


#endif //LAB6_SERVERNODE_H
