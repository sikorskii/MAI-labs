//
// Created by aldes on 02.10.2021.
//

#ifndef LAB3_MTXUTILS_H
#define LAB3_MTXUTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
    int **matrix;
    int size;
} matrix;

int getRandInt();

int** fillFromInput();

int** fillWithRand();

bool prompt();

matrix getMatrix();

void printMatrix(matrix);

#endif //LAB3_MTXUTILS_H
