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

typedef struct matrix {
    int **matrix;
    int size;
} mtx;

int getRandInt();

int** fillFromInput();

int** fillWithRand();

bool prompt();

mtx getMatrix();

mtx getEmptyMatrix(int n);

mtx getReducedMatrix(mtx *matrix, int column, int row);

void printMatrix(mtx* matrix);

void cleanMatrix(mtx* matrix);

int calculateDet(mtx* matrix, int leftBound, int rightBound);

#endif //LAB3_MTXUTILS_H
