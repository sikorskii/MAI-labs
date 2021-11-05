//
// Created by aldes on 30.10.2021.
//

#ifndef LAB1_MEM_H
#define LAB1_MEM_H

#include <fcntl.h>

const char *file1 = "shared1";
const char *file2 = "shared2";
const char *sem1 = "sem1";
const char *sem2 = "sem2";
const char *sem11 = "sem11";
const char *sem22 = "sem22";
const int MAX_LENGTH = 50;
const int NUMBER_OF_BYTES = MAX_LENGTH * sizeof(char);

unsigned accessPerms = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;

void handleError(const char* message) {
    perror(message);
    exit(1);
}


#endif //LAB1_MEM_H
