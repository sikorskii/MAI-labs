//
// Created by aldes on 02.10.2021.
//

#include "mtxutils.h"

#define MAX_ELEMENT 10


int getRandInt() {
    return rand() % MAX_ELEMENT;
}

int** fillWithRand(int n) {
    printf("Matrix will be generated with random\n");
    int **matrix = malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {

        matrix[i] = malloc(n * sizeof(int));

        for (int j = 0; j < n; j++) {

            matrix[i][j] = getRandInt();

        }
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

mtx getReducedMatrix(mtx matrix, int column, int row) {
    int offsetRow = 0;
    int offsetCol = 0;

    mtx newMatrix = getEmptyMatrix(matrix.size - 1);

    for(int i = 0; i < matrix.size - 1; i++) {

        if(i == row) {
            offsetRow++;
        }

        offsetCol = 0;
        for(int j = 0; j < matrix.size - 1; j++) {
            if(j == column) {
                offsetCol = 1;
            }

            newMatrix.matrix[i][j] = matrix.matrix[i + offsetRow][j + offsetCol];
        }
    }

    return newMatrix;

}

void printMatrix(mtx matrix) {
    printf("-----------------Matrix %d by %d-----------------\n", matrix.size);
    for (int i = 0; i < matrix.size; i++) {

        for (int j = 0; j < matrix.size; j++){

            printf("%5d ", matrix.matrix[i][j]);

        }

        printf("\n");

    }
}