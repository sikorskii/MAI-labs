#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../headers/mtxutils.h"
#include "../headers/tutils.h"

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


    for (int i = 0; i < threads_num; i++) {
        thread_args[i].matrix = &matrix;
        thread_args[i].result = 0;
        thread_args[i].left_bound = i * columnsPerThread;
        thread_args[i].right_bound = thread_args[i].left_bound
                + columnsPerThread;
    }

    thread_args[threads_num - 1].left_bound = (threads_num - 1)
            * columnsPerThread;
    thread_args[threads_num - 1].right_bound = matrix.size;

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    createThreads(threads_num, threads, &func, thread_args);

    joinThreads(threads_num, threads);

    long long ans = 0;

    for (int i = 0; i < threads_num; i++) {
        ans += thread_args[i].result;
        printf("ans %d = %lld\n",i, thread_args[i].result);
    }

    gettimeofday(&stop, NULL);

    printf("took %lu mcs\n", (stop.tv_sec - start.tv_sec) *
        100000 + stop.tv_usec - start.tv_usec);

    free(thread_args);

    printf("Multithreading result is %lld\n", ans);

    cleanMatrix(&matrix);

    free(threads);
}
