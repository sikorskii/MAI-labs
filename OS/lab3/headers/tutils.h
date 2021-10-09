//
// Created by aldes on 08.10.2021.
//

#ifndef LAB3_TUTILS_H
#define LAB3_TUTILS_H
#include "mtxutils.h"

typedef struct{
    mtx matrix;
    int bound;
} args;

void createThreads(int threads_num, pthread_t *threads, void *func);

void joinThreads(int threads_num, pthread_t *threads);

#endif //LAB3_TUTILS_H
