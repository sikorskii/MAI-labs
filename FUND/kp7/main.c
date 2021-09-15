//
// Created by djent on 15.05.2021.
//
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "vector.h"


int first(v* CIP, int m) {
    int start = read(CIP, m - 1);
    return start;
}

int last(v* CIP, int m, int size) {
    int end = read(CIP, m);
    if (end == 0) {
        end = size - 1;
    }
    if (end == -1) {
        end = last(CIP, m + 1, size);
    }
    return end;
}

int get(v* CIP, v* PI, v* YE, int m, int n) {
    int start = first(CIP, m);
    int end = last(CIP, m, size(PI));
    int temp = 0;
    bool found = false;

    if (start == -1)
        return temp;

    else {
        for (int i = start; i < end; i++) {
            if (read(PI, i) == n) {
                found = true;
                temp = read(YE, i);
            }
        }
    }

    if (!found)
        return temp;

    return temp;
}

bool ssymmetric(v* CIP, v* PI, v* YE, int m, int n) {
    if (n != m) {
        return false;
    }
    for (int i = 0; i < m; i++) {
        for (int j = i; j < n; j++) {
            int temp1 = get(CIP, PI, YE, i + 1, j + 1);
            int temp2 = get(CIP, PI, YE, j + 1, i + 1);
            if (i == j && temp1 != 0) {
                return false;
            }
            if (temp1 != -temp2) {
                return false;
            }
        }
    }
    return true;
}

void trans(v* CIP, v* PI, v* YE, v* CIP_init, v* PI_init, v* YE_init, int m, int n) {
    int cip_index = 0;
    int pi_index = 0;
    int temp;
    int cnt = 0;

    bool isEmpty = false;
    for (int i = 0; i < m; i++) {
        if (isEmpty) {
            int swap = read(CIP, cip_index - 1);
            push(CIP, swap, cip_index);
            push(CIP, -1, cip_index - 1);
        }
        else {
            push(CIP, cnt, cip_index);
        }
        cip_index++;
        isEmpty = true;
        for (int j = 0; j < n; j++) {
            temp = get(CIP_init, PI_init, YE_init, n - j, m - i);
            if (temp != 0) {
                isEmpty = false;
                push(PI, j + 1, pi_index);
                push(YE, temp, pi_index);
                pi_index++;
                cnt++;
            }
        }
    }
}

void printM(v* CIP, v* PI, v* YE, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", get(CIP, PI, YE, i + 1, j + 1));
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    FILE* f;
    int m;
    int n;
    v* CIP = malloc(sizeof(v));
    v* PI = malloc(sizeof(v));
    v* YE = malloc(sizeof(v));
    if (!(f = fopen(argv[1], "r"))) {
        printf("wrong file");
        exit(1);
    }
    else
        fscanf(f, "%d %d", &m, &n);
        create(CIP, m * n / 2 + 1);
        create(PI, m * n / 2 + 1);
        create(YE, m * n / 2 + 1);
        int row[100];
        int cip_index = 0;
        int pi_index = 0;
        int temp;
        bool isEmpty = true;
        for (int i = 0; i < m; i++) {
            isEmpty = true;
            for (int j = 0; j < n; j++) {
                fscanf(f, "%d", &temp);
                row[j] = temp;
            }
            for (int j = 0; j < n; j++) {
                if (row[j] != 0) {
                    isEmpty = false;
                }
            }
            if (isEmpty) {
                push(CIP, -1, i);
            }
            else {
                push(CIP, cip_index, i);
                for (int j = 0; j < n; j++) {
                    if (row[j] != 0) {
                        push(PI, j + 1, pi_index);
                        push(YE, row[j], pi_index);
                        pi_index++;
                        cip_index++;
                    }
                }
            }
        }

        printf("matrix before::\n");
        printf("v 1: ");
        print(CIP, m);
        printf("v 2: ");
        print(PI, pi_index);
        printf("v 3: ");
        print(YE, pi_index);
        printf("\n\n");
        printM(CIP, PI, YE, m, n);

        v* CIP2 = malloc(sizeof(v));
        v* PI2 = malloc(sizeof(v));
        v* YE2 = malloc(sizeof(v));

        create(CIP2, m * n / 2 + 1);
        create(PI2, m * n / 2 + 1);
        create(YE2, m * n / 2 + 1);

        trans(CIP2, PI2, YE2, CIP, PI, YE, n, m);

        free(CIP); free(PI); free(YE);

        printf("matrix after::\n");
        printf("v 1: ");
        print(CIP2, n);
        printf("v 2: ");
        print(PI2, pi_index);
        printf("v 3: ");
        print(YE2, pi_index);
        printf("\n\n");
        printM(CIP2, PI2, YE2, n, m);

        if (ssymmetric(CIP2, PI2, YE2, m, n))
            printf("symmetric");
        else
            printf("not symmetric");

        free(CIP2); free(PI2); free(YE2);
}
