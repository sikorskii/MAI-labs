//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_SPRINGBOOTAPPLICATION_H
#define LAB6_SPRINGBOOTAPPLICATION_H


#include <set>
#include <mutex>
#include <condition_variable>

#include "ServerNode.h"
#include "AbstractNode.h"
#include "Message.h"
#include "ZmqUtils.h"
#include "MessageBuilder.h"


class SpringBootApplication : public AbstractNode{
public:

    SpringBootApplication() : AbstractNode(-1), created(true) {
        serversId.insert(-1);
    }

    void run() {
        std::thread([this]() {inputProcessing();}).detach();
        std::thread([this]() {messageProcessing();}).detach();
        messageOutput();
    }

private:
    std::set<int> serversId;
    std::mutex creationLock;
    std::condition_variable creationalCondition;
    bool created;

    void inputProcessing() {
        std::string command;
        zmq::socket_t toMessageProcessor(context, zmq::socket_type::push);
        toMessageProcessor.connect(ZmqUtils::MESSAGE_PROCESSOR_URL);

        while (true) {
            std::cin >> command;
            if (command == "test") {
                Message message = MessageBuilder::buildTestMessage();
                //std::cout << "message created with size: " << message.sizeOfBody << std::endl;
                message.sendMessage(toMessageProcessor);
            }
            if (command == "create") {
                Message message = MessageBuilder::buildCreateMessage();
                message.sendMessage(toMessageProcessor);
            }
            if (command == "exit") {
                Message message = MessageBuilder::buildExitMessage();
                //std::cout << "exit message created\n";
                message.sendMessage(toMessageProcessor);
                return;
            }
            if (command == "exec") {
                Message message = MessageBuilder::buildExecMessage();
                message.sendMessage(toMessageProcessor);
            }

        }

    }

    void messageProcessing() {
        zmq::socket_t fromInputProcessor(context, zmq::socket_type::pull);
        zmq::socket_t toOutput(context, zmq::socket_type::push);
        fromInputProcessor.bind(ZmqUtils::MESSAGE_PROCESSOR_URL);
        toOutput.connect(ZmqUtils::MESSAGE_SENDER_URL);
        MessageData data;
        Message toSend;
        while(true) {
            Message receivedMessage;
            receivedMessage.receiveMessage(fromInputProcessor);
            switch (receivedMessage.messageType) {
                case MessageTypes::TEST:
                    toSend = MessageBuilder::buildTestMessage();
                    toSend.sendMessage(toOutput);
                    break;
                case MessageTypes::CREATE_REQUEST:
                    toSend = processCreateMessage(receivedMessage);
                    break;
                case MessageTypes::EXEC_REQUEST:
                    toSend = processExecMessage(receivedMessage);
                    break;
                case MessageTypes::HEARTBIT_REQUEST:
                    break;
                case MessageTypes::QUIT:
                    toSend = MessageBuilder::buildExitMessage();
                    toSend.sendMessage(toOutput);
                    return;
                default:
                    break;
            }
        }
    }

    void messageOutput() {
        zmq::socket_t fromMessageProcessor(context, zmq::socket_type::pull);
        fromMessageProcessor.bind(ZmqUtils::MESSAGE_SENDER_URL);
        Message receivedMessage;
        while (true) {
            receivedMessage.receiveMessage(fromMessageProcessor);
            switch (receivedMessage.messageType) {
                case MessageTypes::TEST:
                    std::cout << "hello from the last one\n";
                    break;
                case MessageTypes::QUIT:
                    return;
                default:
                    std::cout << "another message\n";
                    break;
            }
        }
    }

    void ready() {
        std::unique_lock<std::mutex> lock(creationLock);
        created = true;
        creationalCondition.notify_one();

    }

    void nodeRegister() {
        Message
    }

    Message processCreateMessage(Message& message) {
        MessageData data = MessageBuilder::deserialize(message.body);
        int id = stoi(data.data[0], nullptr);
        int parentId = stoi(data.data[1], nullptr);
        std::cout << "need to create node " << id << "from parent " << parentId << std::endl;
        return create(id, parentId);
    }

    Message create(int id, int pId) {
        if (serversId.find(id) != serversId.end()) {
            std::cout << "already exist" << std::endl;
            return {MessageTypes::CREATE_FAIL, Id, Id, "Already exist"};
        }
        if (serversId.find(pId) == serversId.end()) {
            std::cout << "parent not found" << std::endl;
            return {MessageTypes::CREATE_FAIL, Id, Id, "Parent not found"};
        }
        if (pId == Id) {
            pid_t pid = addChild(id, port);
            serversId.insert(id);
            std::unique_lock<std::mutex> lock(creationLock);
            created = false;
            while(!created)
                creationalCondition.wait(lock);
            std::cout << "awaited" << std::endl;
            return {MessageTypes::CREATE_RESULT, Id, Id, sizeof(pid), (void*)&pid};
        }
        for (auto server: outerNodes) {
            zmq::socket_t requestSocket(context, zmq::socket_type::req);
            requestSocket.connect(ZmqUtils::getOutputAddress(server.second.messageRecieverPort));
            int data[] = {id, pId};
            Message request(MessageTypes::CREATE_REQUEST, Id, server.first, sizeof(data), data);
            Message result;
            request.sendMessage(requestSocket);
            result.receiveMessage(receiverSocket);
            if (result.messageType == MessageTypes::CREATE_RESULT) {
                requestSocket.disconnect(ZmqUtils::getOutputAddress(server.second.messageRecieverPort));
                serversId.insert(id);
                return {MessageTypes::CREATE_RESULT, Id, Id, result.sizeOfBody, result.body};
            }
        }
    }

    Message processExecMessage(Message& message) {
        MessageData data = MessageBuilder::deserialize(message.body);
        if (serversId.find(data.id) == serversId.end()) {
            std::cout << "id " << data.id << " not found" << std::endl;
            return {MessageTypes::EXEC_FAIL, Id, Id, "Node not found"};
        }
        for (auto server : outerNodes) {
            zmq::socket_t request(context, zmq::socket_type::req);
            request.connect(ZmqUtils::getOutputAddress(server.second.messageRecieverPort));
            message.senderId = Id;
            message.recieverId = server.first;
            message.sendMessage(request);
            Message result;
            if (result.messageType == MessageTypes::EXEC_RESULT) {
                return result;
            }
            request.disconnect(ZmqUtils::getOutputAddress(server.second.messageRecieverPort));
        }
        std::cout << "node unaviable" << std::endl;
        return Message(MessageTypes::EXEC_FAIL, Id, Id, "Error node unaviable");
    }
};


#endif //LAB6_SPRINGBOOTAPPLICATION_H
