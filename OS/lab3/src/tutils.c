//
// Created by aldes on 08.10.2021.
//

#include <pthread.h>
#include "../headers/tutils.h"

void createThreads(int threads_num, pthread_t *threads, void *func) {
    for (int i = 0; i < threads_num; i++) {

        if (pthread_create(&threads[i], NULL, func, NULL) != 0) {

            printf("Unable to create %d-th thread\n", i);
            exit(1);

        }

    }
}

void joinThreads(int threads_num, pthread_t *threads) {
    for (int i = 0; i < threads_num; i++) {

        if (pthread_join(threads[i], NULL) != 0) {

            printf("Unable to join %d-th thread\n", i);
            exit(1);

        }
    }
}