//
// Created by aldes on 30.10.2021.
//

#ifndef LAB1_MEM_H
#define LAB1_MEM_H

#include <fcntl.h>

const char *file1 = "sharedFile1";
const char *file2 = "sharedFile2";
const char *sem1 = "sem1";
const char *sem2 = "sem2";

unsigned accessPerms = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;

#endif //LAB1_MEM_H
