//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_MESSAGE_H
#define LAB6_MESSAGE_H

#include "zmq.hpp"
#include <chrono>
#include "MessageTypes.h"


class Message {
public:
    Message(MessageTypes messageType, int senderId, int receiverId, int _sizeOfBody, void *_body) : messageType(
            messageType), senderId(senderId), recieverId(receiverId), sizeOfBody(_sizeOfBody), body(nullptr) {
        if (sizeOfBody > 0) {
            body = malloc(sizeOfBody);
            memcpy(body, _body, _sizeOfBody);
            //std::cout <<  "_body " << ((int*)_body)[0] << "\n";
            //std::cout << "body " << ((int*)body)[0] << "\n";
        }
    }

    Message(): Message(MessageTypes::EMPTY, -1, -1, 0, nullptr) {
    }

    Message(MessageTypes messageType, int senderId, int32_t receiverId):
            Message(messageType, senderId, receiverId, 0, nullptr)
    {
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

    ~Message() {
        if (body != nullptr)
            operator delete(body);
        body = nullptr;
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
        int64_t q = 0;
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
