//
// Created by aldes on 08.10.2021.
//

#include <pthread.h>
#include "../headers/tutils.h"

void *func(void *args) {
    arg_t *args_s = (arg_t*) args;
    printf("This is thread %lu\n", pthread_self());
    printf("My matrix size is %d, my right bound is %d\n",
           args_s->matrix->size, args_s->right_bound);

    args_s->result = calculateDet(args_s->matrix,
                                  args_s->left_bound,args_s->right_bound);
    pthread_exit(NULL);
}

void createThreads(int threads_num, pthread_t *threads, void *func, arg_t *targs) {
    for (int i = 0; i < threads_num; i++) {

        if (pthread_create(&threads[i], NULL, func, (void*)&targs[i]) != 0) {

            printf("Unable to create %d-th thread\n", i);
            free(threads);
            free(targs);
            exit(1);

        }

    }
}

void joinThreads(int threads_num, pthread_t *threads) {
    for (int i = 0; i < threads_num; i++) {

        if (pthread_join(threads[i], NULL) != 0) {

            printf("Unable to join %d-th thread\n", i);
            free(threads);
            exit(1);

        }
    }
}