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

int getRandInt();

int** fillFromInput();

int** fillWithRand();

bool prompt();

int** getMatrix();

void printMatrix(int **matrix, int n);

#endif //LAB3_MTXUTILS_H
