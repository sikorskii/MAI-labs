#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "../headers/mtxutils.h"
#include "../headers/tutils.h"

typedef struct arguments {
    mtx *matrix;
    int i;
} arg_t;

void *func(void *args) {
    arg_t *args_s = (arg_t*) args;
    printf("This is thread %lu\n", pthread_self());
    pthread_exit(NULL);
}

int main(int argc, char** argv) {

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

    printf("Colums per thread: %d\n Threads: %d, Matrix.size: %d\n",
           columnsPerThread, threads_num, matrix.size);

    pthread_t *threads = calloc(threads_num, sizeof(pthread_t));

    arg_t *thread_args = calloc(threads_num, sizeof(arg_t));

    createThreads(threads_num, threads, &func);

    joinThreads(threads_num, threads);

    for (int i = 0; i < matrix.size; i++) {
        mtx newMatrix = getReducedMatrix(&matrix, i, 0);
        //printMatrix(&newMatrix);
        cleanMatrix(&newMatrix);
    }

    printf("Calcucated det is: %d\n", calculateDet(&matrix));

    cleanMatrix(&matrix);

    free(threads);
}
