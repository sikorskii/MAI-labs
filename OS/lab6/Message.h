//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_MESSAGE_H
#define LAB6_MESSAGE_H

#include "zmq.hpp"
#include <chrono>
#include <thread>
#include "MessageTypes.h"


class Message {
public:
    Message(MessageTypes messageType, int senderId, int receiverId, int _sizeOfBody, void *_body) : messageType(
            messageType), senderId(senderId), recieverId(receiverId), sizeOfBody(_sizeOfBody), body(nullptr) {
        if (sizeOfBody > 0) {
            body = malloc(sizeOfBody);
            memcpy(body, _body, _sizeOfBody);
        }
    }

    Message(): Message(MessageTypes::EMPTY, -1, -1, 0, nullptr) {
    }

    Message(MessageTypes messageType, int senderId, int receiverId):
            Message(messageType, senderId, receiverId, 0, nullptr)
    {
    }
    ~Message() {
        if (body != nullptr) {
            free(body);
            body = nullptr;
        }
    }

    Message(Message&& message) noexcept {
        messageType = message.messageType;
        senderId = message.senderId;
        recieverId = message.recieverId;
        sizeOfBody = message.sizeOfBody;
        body = message.body;
        message.body = nullptr;
    }

    Message& operator= (Message&& message) noexcept {
        if (body != nullptr) {
            free(body);
            body = nullptr;
        }

        messageType = message.messageType;
        senderId = message.senderId;
        recieverId = message.recieverId;
        sizeOfBody = message.sizeOfBody;
        body = message.body;
        message.body = nullptr;
        return *this;
    }
    Message(MessageTypes messageType, int senderId, int receiverId, const char* str):
            Message(messageType, senderId, receiverId)
    {
        sizeOfBody = strlen(str) + 1;
        if (sizeOfBody > 0) {
            body = operator new(sizeOfBody);
            memcpy(body, (void*)str, sizeOfBody);
        }
    }

    void sendMessage(zmq::socket_t& socket) {
        int header[] = {senderId, recieverId, sizeOfBody};
        zmq::message_t typeOfMessage(&messageType, sizeof(messageType));
        zmq::message_t messageHeader(&header, sizeof(header));
        zmq::message_t messageBody(body, sizeOfBody);
        //send compound message using SNDMORE flag in every message part except the last one
        socket.send(typeOfMessage, zmq::send_flags::sndmore);
        socket.send(messageHeader, zmq::send_flags::sndmore);
        socket.send(messageBody, zmq::send_flags::none);
    }

    void receiveMessage(zmq::socket_t& socket) {
        zmq::message_t receivedType;
        zmq::message_t receivedHeader;
        zmq::message_t receivedBody;
        for (bool t = false; !t; t = getMessage(socket, receivedType, receivedHeader, receivedBody)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        messageType = *(MessageTypes*)receivedType.data();
        senderId = ((int*)receivedHeader.data())[0];
        recieverId = ((int*)receivedHeader.data())[1];
        sizeOfBody = ((int*)receivedHeader.data())[2];
        operator delete(body);
        body = nullptr;
        if (sizeOfBody > 0) {
            body = operator new(sizeOfBody);
            memcpy(body, receivedBody.data(), sizeOfBody);
        }
    }

    bool receiveMessage(zmq::socket_t& socket, std::chrono::milliseconds time) {
        zmq::message_t receivedType;
        zmq::message_t receivedHeader;
        zmq::message_t receivedBody;
        unsigned long long passed = 0;
        bool t = false;
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
        for ( ; !t && passed < time.count(); t = getMessage(socket, receivedType, receivedHeader, receivedBody)) {
            passed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        if (!t)
            return false;
        messageType = *(MessageTypes*)receivedType.data();
        senderId = ((int*)receivedHeader.data())[0];
        recieverId = ((int*)receivedHeader.data())[1];
        sizeOfBody = ((int*)receivedHeader.data())[2];
        operator delete(body);
        body = nullptr;
        if (sizeOfBody > 0) {
            body = operator new(sizeOfBody);
            memcpy(body, receivedBody.data(), sizeOfBody);
        }
        return true;
    }

    void update(int senderID, int receiverId) {
        senderId = senderID;
        recieverId = receiverId;
    }

    MessageTypes messageType;
    int senderId;
    int recieverId;
    int sizeOfBody;
    void* body;
private:
    bool getMessage(zmq::socket_t& socket, zmq::message_t& typeOfMessage,
                    zmq::message_t& header, zmq::message_t& bodyOfMessage) {
        return (socket.recv(typeOfMessage, zmq::recv_flags::dontwait) &&
        socket.recv(header, zmq::recv_flags::dontwait) &&
        socket.recv(bodyOfMessage, zmq::recv_flags::dontwait));
    }
};


#endif //LAB6_MESSAGE_H
