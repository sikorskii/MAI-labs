#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "mtxutils.h"



int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Not enough args. Please specify the number of threads: ./executable <threads number>\n");
        exit(1);
    }

    int threads_num = (int)strtol(argv[1], NULL, 10);

    matrix matrix;
    matrix = getMatrix();
}
