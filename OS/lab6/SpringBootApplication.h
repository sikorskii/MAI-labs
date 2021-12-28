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
        std::thread([this]() {registrator();}).detach();
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
            if (command == "create") {
                Message message = MessageBuilder::buildCreateMessage(Id);
                message.sendMessage(toMessageProcessor);
            }
            if (command == "exit") {
                Message message = MessageBuilder::buildExitMessage(Id);
                message.sendMessage(toMessageProcessor);
                break;
            }
            if (command == "exec") {
                Message message = MessageBuilder::buildExecMessage();
                message.sendMessage(toMessageProcessor);
            }
            if (command == "hb") {
                int time;
                std::cin >> time;
                Message message = MessageBuilder::buildPingRequest(time, -1);
                message.sendMessage(toMessageProcessor);
            }
            else {
                std::cin.clear();
            }

        }

    }

    void messageProcessing() {
        zmq::socket_t fromInputProcessor(context, zmq::socket_type::pull);
        zmq::socket_t toOutput(context, zmq::socket_type::push);
        fromInputProcessor.bind(ZmqUtils::MESSAGE_PROCESSOR_URL);
        toOutput.connect(ZmqUtils::MESSAGE_SENDER_URL);
        MessageData data;
        std::thread pl;
        Message toSend;
        bool quit = false;
        while(!quit) {
            Message receivedMessage;
            receivedMessage.receiveMessage(fromInputProcessor);
            int time;
            std::thread th;
            switch (receivedMessage.messageType) {
                case MessageTypes::CREATE_REQUEST:
                    toSend = processCreateMessage(std::move(receivedMessage));
                    toSend.sendMessage(toOutput);
                    break;
                case MessageTypes::EXEC_REQUEST:
                    toSend = processExecMessage(std::move(receivedMessage));
                    toSend.sendMessage(toOutput);
                    break;
                case MessageTypes::HEARTBIT_REQUEST:
                    time = *(int*)receivedMessage.body;
                    pl = std::thread([this, &time, &quit]() { pingListener(*this, time, quit); });
                    pl.detach();
                    processPingRequest(time);
                    break;
                case MessageTypes::QUIT:
                    toSend = exitProcessor(std::move(receivedMessage), quit);
                    quit = true;
                    toSend.sendMessage(toOutput);
                    return;
                default:
                    break;
            }
        }
    }

    static void pingListener(SpringBootApplication& that, int& time, bool& exit) {
        while(!exit) {
            for (auto server: that.outerNodes) {
                zmq::socket_t sender(that.context, zmq::socket_type::req);
                sender.connect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
                Message pingRequest = MessageBuilder::buildPingRequest(time, server.first);
                try {
                    pingRequest.sendMessage(sender);
                    if (!pingRequest.receiveMessage(sender, std::chrono::milliseconds(time * 4))) {
                        std::cout << "node " << server.first << " and all her children dead" << std::endl;
                        //that.outerNodes.erase(server.first);
                    }
                }
                catch (const zmq::error_t& error) {
                    continue;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(time));
            }
        }
    }

    void processPingRequest(int time) {
        for (auto server: outerNodes) {
            Message pingRequest = MessageBuilder::buildPingRequest(time, Id);
            zmq::socket_t socket(context, zmq::socket_type::req);
            socket.connect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
            pingRequest.sendMessage(socket);
            pingRequest.receiveMessage(socket, std::chrono::milliseconds(100));
            //std::cout << "message sent to node " << server.first << std::endl;
            socket.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
        }
    }

    void messageOutput() {
        zmq::socket_t fromMessageProcessor(context, zmq::socket_type::pull);
        fromMessageProcessor.bind(ZmqUtils::MESSAGE_SENDER_URL);
        Message receivedMessage;
        bool quit{false};
        while (!quit) {
            receivedMessage.receiveMessage(fromMessageProcessor);
            switch (receivedMessage.messageType) {
                case MessageTypes::CREATE_RESULT:
                    createOutput(receivedMessage);
                    break;
                case MessageTypes::EXEC_RESULT:
                    execOutput(receivedMessage);
                    break;
                case MessageTypes::QUIT:
                    std::cout << "EXIT" << std::endl;
                    quit = true;
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

    void registrator() {
        Message messageIn;
        Message messageOut;
        while (true) {
            messageIn.receiveMessage(Receiver);
            if (messageIn.messageType == MessageTypes::QUIT) {
                messageIn.sendMessage(Receiver);
                break;
            }
            messageOut = processRegisterMessage(messageIn);
            ready();
            messageOut.sendMessage(Receiver);

        }
    }


    Message processRegisterMessage(Message& message) {
        auto body = (char*)message.body;
        pid_t pid = *(pid_t*)(body);
        int recPort = *(int*)(body + sizeof(pid_t));
        int regPort = *(int*)(body + sizeof(pid_t) + sizeof(int));
        outerNodes.emplace(message.senderId, ChildNodeInfo(pid, recPort, regPort));
        return {MessageTypes::RELATE_RESULT, Id, message.senderId};
    }

    Message processCreateMessage(Message&& message) {
        int id = ((int*)message.body)[0];
        int parentId = ((int*)message.body)[1];
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
            pid_t pid = addChild(id, Port);
            std::cout << "received Pid: " << pid << std::endl;
            serversId.insert(id);
            std::unique_lock<std::mutex> lock(creationLock);
            created = false;
            while(!created) {
                creationalCondition.wait(lock);
            }
            std::cout << "awaited" << std::endl;
            return {MessageTypes::CREATE_RESULT, Id, Id, sizeof(pid), (void*)&pid};
        }
        for (auto server: outerNodes) {
            zmq::socket_t requestSocket(context, zmq::socket_type::req);
            requestSocket.connect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
            int data[] = {id, pId};
            Message request(MessageTypes::CREATE_REQUEST, Id, server.first, sizeof(data), data);
            Message result;
            request.sendMessage(requestSocket);
            //result.receiveMessage(requestSocket);
            if (!result.receiveMessage(requestSocket, std::chrono::milliseconds(2000))) {
                requestSocket.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
                continue;
            }
            if (result.messageType == MessageTypes::CREATE_RESULT) {
                requestSocket.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
                serversId.insert(id);
                return {MessageTypes::CREATE_RESULT, Id, Id, result.sizeOfBody, result.body};
            }
            requestSocket.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
        }
        return {MessageTypes::CREATE_FAIL, Id, Id, "Parent is uavailable"};
    }

    Message processExecMessage(Message&& message) {
        MessageData data = MessageBuilder::deserialize(message.body);
        if (serversId.find(data.id) == serversId.end()) {
            std::cout << "id " << data.id << " not found" << std::endl;
            return {MessageTypes::EXEC_FAIL, Id, Id, "Node not found"};
        }
        for (auto server : outerNodes) {
            zmq::socket_t request(context, zmq::socket_type::req);
            request.connect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
            message.senderId = Id;
            message.recieverId = server.first;
            message.sendMessage(request);
            Message result;
            //result.receiveMessage(request);
            if (!result.receiveMessage(request, std::chrono::milliseconds(2000))) {
                request.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
                continue;
            }
            if (result.messageType == MessageTypes::EXEC_RESULT) {
                result.update(Id, Id);
                return result;
            }
            request.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
        }
        std::cout << "node unaviable" << std::endl;
        return Message(MessageTypes::EXEC_FAIL, Id, Id, "node unaviable");
    }

    Message exitProcessor(Message&& message, bool& exit) {
        zmq::socket_t exitRegister(context, zmq::socket_type::req);
        exitRegister.connect(ZmqUtils::getOutputAddress(Port));
        Message messageExit(MessageTypes::QUIT, Id, Id);
        Message temporary;
        messageExit.sendMessage(exitRegister);
        temporary.receiveMessage(exitRegister);
        exitRegister.disconnect(ZmqUtils::getOutputAddress(Port));
        std::cout << "closing begins" << std::endl;
        exit = true;
        for (auto server: outerNodes) {
            zmq::socket_t request(context, zmq::socket_type::req);
            request.connect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
            messageExit.sendMessage(request);
            messageExit.receiveMessage(request, std::chrono::milliseconds(2000));
            //messageExit.sendMessage(Receiver);
            Message exitResult;
            //messageExit.receiveMessage(Receiver, std::chrono::milliseconds(1000));
            //exitResult.receiveMessage(request);
            request.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
        }
        std::cout << "servers dead" << std::endl;
        return messageExit;
    }

    void createOutput(Message& message) {
        std::cout << "Ok:" << *(int*)message.body << std::endl;
    }

    void execOutput(Message& message) {
        MessageData data = MessageBuilder::deserialize(message.body);
        std::cout << "Exec Ok: " << data.id << " " << data.data[0] << " " << data.data[1] << std::endl;
    }
};


#endif //LAB6_SPRINGBOOTAPPLICATION_H
