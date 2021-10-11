//
// Created by aldes on 08.10.2021.
//

#ifndef LAB3_TUTILS_H
#define LAB3_TUTILS_H
#include "mtxutils.h"

void *func(void *args);

typedef struct arguments {
    mtx *matrix;
    int left_bound;
    int right_bound;
    long long result;
} arg_t;


void createThreads(int threads_num, pthread_t *threads,
                   void *func, arg_t * targs);

void joinThreads(int threads_num, pthread_t *threads);

#endif //LAB3_TUTILS_H
