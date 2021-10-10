#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "../headers/mtxutils.h"
#include "../headers/tutils.h"



void *func(void *args) {
    arg_t *args_s = (arg_t*) args;
    printf("This is thread %lu\n", pthread_self());
    printf("My matrix size is %d, my right bound is %d\n", args_s->matrix->size, args_s->right_bound);
    args_s->result = calculateDet(args_s->matrix, args_s->left_bound, args_s->right_bound);
    pthread_exit(NULL);
}

signed main(signed argc, char** argv) {

    if (argc != 2) {
        printf("Not enough args. Please specify the number of threads: "
               "./executable <threads number>\n");
        exit(1);
    }

    int threads_num = (int)strtol(argv[1], NULL, 10);

    mtx matrix;
    matrix = getMatrix();
    printMatrix(&matrix);

    int columnsPerThread = matrix.size / threads_num;

    if (columnsPerThread < 1) {

        printf("There is too many threads for this matrix, "
               "number of threads reduced: %d -> %d\n", threads_num,
               matrix.size);
        threads_num = matrix.size;

    }

    columnsPerThread = matrix.size / threads_num;

    printf("Colums per thread: %d\n Threads: %d, Matrix.size: %d\n",
           columnsPerThread, threads_num, matrix.size);

    pthread_t *threads = calloc(threads_num, sizeof(pthread_t));

    arg_t *thread_args = calloc(threads_num, sizeof(arg_t));

    arg_t testarg = {&matrix, 10, 10, 333};

    for (int i = 0; i < threads_num; i++) {
        thread_args[i].matrix = &matrix;
        thread_args[i].result = 0;
        thread_args[i].left_bound = i * columnsPerThread;
        thread_args[i].right_bound = thread_args[i].left_bound + columnsPerThread;
    }

    thread_args[threads_num - 1].left_bound = (threads_num - 1) * columnsPerThread;
    thread_args[threads_num - 1].right_bound = matrix.size;

    for (int i = 0; i < threads_num; i++) {
        //printf("size %d left %d right %d\n", thread_args[i].matrix->size, thread_args[i].left_bound, thread_args[i].right_bound);
    }

    for (int i = 0; i < threads_num; i++) {

        if (pthread_create(&threads[i], NULL, func, (void*)&thread_args[i]) != 0) {

            printf("Unable to create %d-th thread\n", i);
            exit(1);

        }

    }

    //createThreads(threads_num, threads, &func, thread_args);

    joinThreads(threads_num, threads);

    long long ans = 0;
    int degree = 1;
    for (int i = 0; i < threads_num; i++) {
        ans += thread_args[i].result;
        printf("ans i = %lld\n", thread_args[i].result);
    }
    free(thread_args);

    for (int i = 0; i < matrix.size; i++) {
        mtx newMatrix = getReducedMatrix(&matrix, 0, i);
        //printMatrix(&newMatrix);
        cleanMatrix(&newMatrix);
    }

    printf("Calcucated det is: %lld\n", calculateDet(&matrix, 0, matrix.size));
    printf("Multithreading result is %lld\n", ans);

    cleanMatrix(&matrix);

    free(threads);
}
