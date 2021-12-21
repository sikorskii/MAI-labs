//
// Created by aldes on 15.12.2021.
//

#ifndef LAB6_MESSAGEBUILDER_H
#define LAB6_MESSAGEBUILDER_H

#include <iostream>
#include "Message.h"
#include "MessageData.h"

class MessageBuilder {
public:
    static Message buildCreateMessage(int Id) {
        int data[2];
        std::cin >> data[0] >> data[1];
        std::cout << "add new node " << data[0] << " to parent " << data[1] << std::endl;
        return {MessageTypes::CREATE_REQUEST, Id, Id, sizeof(data), (void*)&data};
    }

    static Message buildPingRequest(int time, int id) {
        return {MessageTypes::HEARTBIT_REQUEST, id, -1, sizeof(int), (void*)&time};
    }

    static Message buildPingMessage(unsigned long long time, int Id) {
        return {MessageTypes::HEARTBIT_RESULT, Id, -1, sizeof(unsigned long long), (void*)&time};
    }


    static Message buildTestMessage() {
        const char *testMessage = "first";
        return {MessageTypes::TEST, -1, -1, testMessage};
    }

    static Message buildExitMessage(int Id) {
        return {MessageTypes::QUIT, Id, Id};
    }

    static Message buildExecMessage() {
        int id;
        std::cin >> id;
        std::vector<std::string> data(2);
        std::cin >> data[0] >> data[1];
        std::cout << id << " " << data[0] << " " << data[1] << "\n";
        void* body = serialize(id, data);
        return {MessageTypes::EXEC_REQUEST, -1, id, getSize(data), body};
    }

    static int getSize(std::vector<std::string>& vector) {
        int size = 3 * sizeof(int);
        for (auto x : vector) {
            size += x.size() * sizeof(char);
        }
        return size;
    }

    static void* serialize(int& id, std::vector<std::string> &data) {
        void* body = malloc(getSize(data));
        memcpy(body, &id, sizeof(int));
        int size1 = data[0].size();
        int size2 = data[1].size();
        int offset = sizeof(int);
        memcpy((char*)body + offset, &size1, sizeof(int));
        offset += sizeof(int);
        memcpy((char*)body + offset, data[0].c_str(), size1 * sizeof(char));
        offset += size1 * sizeof(char);
        memcpy((char*) body + offset, &size2, sizeof(int));
        offset += sizeof(int);
        memcpy((char*) body + offset, data[1].c_str(), size2 * sizeof(char));
        return body;
    }

    static MessageData deserialize(void* messageBody) {
        MessageData data;
        data.id = *(int*)((char*)(messageBody));
        int offset = sizeof(int);
        int size1 = *(int*)((char*)messageBody + offset);
        void* str1 = malloc(size1);
        offset += sizeof(int);
        memcpy(str1, ((char*)messageBody + offset), size1 * sizeof(char));
        offset += size1 * sizeof(char);
        int size2 = *(int*)((char*)messageBody + offset);
        offset += sizeof(int);
        void* str2 = malloc(size2);
        memcpy(str2, ((char*)messageBody + offset), size2 * sizeof(char));
        //std::cout << data.id << " " << size1 << " "  << (char*)str1 << " " << size2 << " " << (char*)str2 << "\n";
        data.len1 = size1;
        data.len2 = size2;
        data.data.emplace_back((char*)str1);
        data.data.emplace_back((char*)str2);
        return data;
    }
};


#endif //LAB6_MESSAGEBUILDER_H
