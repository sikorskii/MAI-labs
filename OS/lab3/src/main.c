#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "mtxutils.h"
#include "tutils.h"

void *func(void *arg) {
    //printf("This is thread %lu\n", pthread_self());
    pthread_exit(NULL);
}

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Not enough args. Please specify the number of threads: ./executable <threads number>\n");
        exit(1);
    }

    int threads_num = (int)strtol(argv[1], NULL, 10);

    mtx matrix;
    matrix = getMatrix();
    printMatrix(matrix);

    pthread_t *threads = calloc(threads_num, sizeof(pthread_t));

    for (int i = 0; i < threads_num; i++) {
        if (pthread_create(&threads[i], NULL, &func, NULL) != 0) {
            printf("Unable to create %d-th thread\n", i);
            exit(1);
        }
    }

    for (int i = 0; i < threads_num; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Unable to join %d-th thread\n", i);
            exit(1);
        }
    }

    for (int i = 0; i < matrix.size; i++) {
        mtx newMatrix = getReducedMatrix(matrix, i, 0);
        printMatrix(newMatrix);
    }

    free(threads);
}
