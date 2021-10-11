//
// Created by aldes on 02.10.2021.
//
#include "../headers/mtxutils.h"

#define MAX_ELEMENT 10

int getRandInt() {
    return rand() % MAX_ELEMENT;
}

int** fillWithRand(int n) {
    printf("Matrix will be generated with random\n");
    int **matrix = malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {

        matrix[i] = malloc(n * sizeof(int));

        for (int j = 0; j < n; j++)

            matrix[i][j] = getRandInt();

    }

    return matrix;
}

int** fillFromInput(int n) {
    int **matrix = malloc(n * sizeof(int*));
    printf("Enter %d elements of matix:\n", n * n);
    for (int i = 0; i < n; i++) {

        matrix[i] = malloc(n * sizeof(int));

        for (int j = 0; j < n; j++) {

            scanf("%d", &matrix[i][j]);

        }
    }

    return matrix;
}

bool prompt() {
    printf("Fill mtx with random values?\n Enter 1/0 \n");
    int mode;
    scanf("%d", &mode);

    switch (mode) {
        case 1 :
            return true;

        case 0 :
            return false;

        default :
            return prompt();
    }
}

mtx getMatrix() {
    int n;
    printf("Enter mtx's dimension: \n");
    scanf("%d", &n);
    mtx matrix;
    matrix.size = n;
    matrix.matrix = NULL;

    if (prompt()) {

        srand(time(NULL));
        matrix.matrix = fillWithRand(n);

    }
    else {

        matrix.matrix = fillFromInput(n);

    }

    return matrix;

}

mtx getEmptyMatrix(int n) {
    mtx matrix;
    matrix.size = n;
    matrix.matrix = malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++)
        matrix.matrix[i] = malloc(n * sizeof(int));


    return matrix;

}

mtx getReducedMatrix(mtx *matrix, int i, int j) {

    mtx newMatrix = getEmptyMatrix(matrix->size - 1);

    for (int a = 0, x = 0; a < matrix->size - 1; a++, x++) {

        for (int b = 0, y = 0; b < matrix->size - 1; b++, y++) {

            if (x == i)
                x++;
            if (y == j)
                y++;

            newMatrix.matrix[a][b] = matrix->matrix[x][y];
        }
    }

    return newMatrix;

}

void printMatrix(mtx *matrix) {
    printf("-----------------Matrix %d by %d-----------------\n",
            matrix->size, matrix->size);

    for (int i = 0; i < matrix->size; i++) {

        for (int j = 0; j < matrix->size; j++){

            printf("%5d ", matrix->matrix[i][j]);

        }

        printf("\n");

    }
}

void cleanMatrix(mtx* matrix) {
    for (int i = 0; i < matrix->size; i++)
        free(matrix->matrix[i]);

    free(matrix->matrix);
}

long basicDeterminant(mtx *matrix) {

    long long d = 0;

    if (matrix->size == 2) {
        return matrix->matrix[0][0] * matrix->matrix[1][1]
            - matrix->matrix[0][1] * matrix->matrix[1][0];
    }

    for (int i = 0; i < matrix->size; i++)
        d += calculateMinor(i, 0, matrix->matrix[i][0], matrix);

    return d;
}

long long calculateMinor(int i, int j, int a, mtx* matrix) {
    int degree;
    mtx submatrix = getReducedMatrix(matrix, i, 0);

    if ((i + j) % 2 == 0)
        degree = 1;
    else
        degree = -1;

    long det = degree * a * basicDeterminant(&submatrix);

    cleanMatrix(&submatrix);

    return det;
}

long long calculateDet(mtx* matrix, int leftBound, int rightBound) {
    long long det = 0;

    if(matrix->size == 1) {
        return matrix->matrix[0][0];
    }

    else if(matrix->size == 2) {
        return matrix->matrix[0][0] * matrix->matrix[1][1]
                - matrix->matrix[0][1] * matrix->matrix[1][0];
    }

    else {

        for(int j = leftBound; j < rightBound; j++) {

            long long temp = calculateMinor(j, 0, matrix->matrix[j][0], matrix);
            det += temp;
        }
    }

    return det;
}