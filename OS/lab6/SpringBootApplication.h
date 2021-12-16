//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_SPRINGBOOTAPPLICATION_H
#define LAB6_SPRINGBOOTAPPLICATION_H


#include <set>

#include "ServerNode.h"
#include "AbstractNode.h"
#include "Message.h"
#include "ZmqUtils.h"
#include "MessageBuilder.h"


class SpringBootApplication : public AbstractNode{
public:
    void run() {
        std::thread([this]() {inputProcessing();}).detach();
        std::thread([this]() {messageProcessing();}).detach();
        messageOutput();
    }

private:
    std::set<int> serversId;

    void inputProcessing() {
        std::string command;

        zmq::socket_t toMessageProcessor(context, zmq::socket_type::push);
        toMessageProcessor.connect(ZmqUtils::MESSAGE_PROCESSOR_URL);

        while (true) {
            std::cin >> command;
            if (command == "test") {
                Message message = MessageBuilder::buildTestMessage();
                std::cout << "message created with size: " << message.sizeOfBody << std::endl;
                message.sendMessage(toMessageProcessor);
            }
            if (command == "exit") {
                Message message = MessageBuilder::buildExitMessage();
                std::cout << "exit message created\n";
                message.sendMessage(toMessageProcessor);
                return;
            }
            if (command == "exec") {
                Message message = MessageBuilder::buildExecMessage();
                MessageData data = MessageBuilder::deserialize(message.body);
                std::cout << "exec message created with pattern = " << data.data[1] << std::endl;
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
                    break;
                case MessageTypes::EXEC_REQUEST:
                    data = MessageBuilder::deserialize(receivedMessage.body);
                    std::cout << "exec message recieved " << data.data[0] << " " << data.data[1] << std::endl;
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
};


#endif //LAB6_SPRINGBOOTAPPLICATION_H
