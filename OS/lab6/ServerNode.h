//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_SERVERNODE_H
#define LAB6_SERVERNODE_H


#include "SpringBootApplication.h"
#include "AbstractNode.h"
#include "Message.h"
#include <chrono>
#include "MessageData.h"
#include "MessageBuilder.h"

class ServerNode : public AbstractNode{
public:
    ServerNode(int id, int _parentId, int _parentPort, int _registerPort) :
            AbstractNode(id), parentId(_parentId), parentPort(_parentPort), parentRegPort(_registerPort),
            registerSocket(context, zmq::socket_type::rep) {
        occupyPort();
        registerPort = ZmqUtils::occupyPort(registerSocket);
        selfRelation(parentRegPort);
    }

    void run() {
        std::cout << "SERVER CREATED" << std::endl;
        //std::cout << "port " << Port << " parent port " << parentPort << std::endl;
        std::thread([this]() {registrator();}).detach();
        bool quit = false;
        bool ping = false;
        while (true) {
            Message messageOut;
            Message messageIn;
            //std::cout << "before recieving" << std::endl;
            if (!messageIn.receiveMessage(Receiver, std::chrono::milliseconds(100))) {
                continue;
            }
            //std::cout << "after recevinge" << std::endl;
            int time;
            std::thread th;
            switch (messageIn.messageType) {
                case MessageTypes::CREATE_REQUEST:
                    messageOut = createProcessor(messageIn);
                    messageOut.sendMessage(Receiver);
                    break;
                case MessageTypes::EXEC_REQUEST:
                    messageOut = execProcessor(messageIn);
                    messageOut.sendMessage(Receiver);
                    break;
                case MessageTypes::HEARTBIT_REQUEST:
                    //std::cout << "Heartbit request arrived at server " << Id << std::endl;
                    time = *(int*)messageIn.body;
                    processPingRequest(time);
                    if (!ping) {
                        ping = true;
                        th = std::thread([this, &time, &quit]() { bitGenerator(*this, time, quit); });
                        th.detach();
                    }
                    messageIn.sendMessage(Receiver);
                    break;
                case MessageTypes::QUIT:
                    messageOut = quitProcessor(messageIn);
                    messageOut.sendMessage(Receiver);
                    quit = true;
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

    static void bitGenerator(ServerNode& that, int& time, bool& exit) {
        while (!exit) {
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
            pingRequest.receiveMessage(socket, std::chrono::milliseconds(1000));
            //std::cout << "message sent to node " << server.first << std::endl;
            socket.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
        }
    }
    void registrator() {
        Message messageIn;
        std::cout << "from registrator " << getpid() << std::endl;
        Message messageOut;
        while (true) {
            messageIn.receiveMessage(registerSocket);
            std::cout << "register message got" << std::endl;
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
        void* relationBody = createRelation(getpid(), Port, registerPort);
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
            sender.connect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
            int data[] = {id, parId};
            Message request(MessageTypes::CREATE_REQUEST, Id, server.first, sizeof(data), (void*)data);
            Message result;
            request.sendMessage(sender);
            //result.receiveMessage(sender);
            if (!result.receiveMessage(sender, std::chrono::milliseconds(2000))) {
                sender.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
                continue;
            }
            if (result.messageType == MessageTypes::CREATE_RESULT) {
                pid = *(int*)result.body;
                sender.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
                break;
            }
            sender.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
        }
        if (pid == -1) {
            return {MessageTypes::CREATE_FAIL, Id, sndId, sizeof(pid), &pid};
        }
        return {MessageTypes::CREATE_RESULT, Id, sndId, sizeof(pid), &pid};
    }

    Message execProcessor(Message& message) {
        MessageData data = MessageBuilder::deserialize(message.body);
        std::string text = data.data[0];
        std::string pattern = data.data[1];
        //std::cout << "EXEC INFO: Id " << Id << "target id " << data.id << std::endl;
        if (data.id == Id) {
            text = text.substr(0, data.len1);
            pattern = pattern.substr(0, data.len2);
            size_t found = text.find(pattern, 0);
            std::string answer;
            if (found != std::string::npos) {
                while (found != std::string::npos) {
                    answer += std::to_string(found);
                    answer += " ";
                    found = text.find(pattern, found + 1);
                }
                std::cout << answer << std::endl;
            }
            else {
                std::cout << "NOT FOUND!" << std::endl;
            }
            std::vector<std::string> newData(2);
            newData[0] = answer;
            newData[1] = "";
            void* newBody = MessageBuilder::serialize(data.id, newData);
            int newSize = MessageBuilder::getSize(newData);
            std::cout << "RECEVIEd!! " << text << " " << pattern << std::endl;
            return {MessageTypes::EXEC_RESULT, Id, parentId, newSize, newBody};
        }
        for (auto server : this->outerNodes) {
            zmq::socket_t requester(context, zmq::socket_type::req);
            requester.connect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
            message.update(Id, server.first);
            message.sendMessage(requester);
            Message result;
            //result.receiveMessage(requester);
            if (!result.receiveMessage(requester, std::chrono::milliseconds(2000))) {
                requester.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
                continue;
            }
            if (result.messageType == MessageTypes::EXEC_RESULT) {
                result.update(Id, parentId);
                requester.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
                return result;
            }
            requester.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
        }
        return {MessageTypes::EXEC_FAIL, Id, parentId, "node is unaviable"};
    }

    Message quitProcessor(Message& message) {
        Message messageQuit(MessageTypes::QUIT, Id, Id);
        for (auto server: outerNodes) {
            zmq::socket_t request(context, zmq::socket_type::req);
            request.connect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
            messageQuit.sendMessage(request);
            Message messageResult;
            //messageResult.receiveMessage(request);
            if (!messageResult.receiveMessage(request, std::chrono::milliseconds(2000))) {
                request.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
                continue;
            }
            request.disconnect(ZmqUtils::getOutputAddress(server.second.ReceiverPort));
        }
        zmq::socket_t request(context, zmq::socket_type::req);
        request.connect(ZmqUtils::getOutputAddress(registerPort));
        messageQuit.sendMessage(request);
        //messageQuit.receiveMessage(request);
        messageQuit.receiveMessage(request, std::chrono::milliseconds(2000));
        request.disconnect(ZmqUtils::getOutputAddress(registerPort));
        return messageQuit;
    }

};


#endif //LAB6_SERVERNODE_H
