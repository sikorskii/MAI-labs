//
// Created by djent on 15.05.2021.
//
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
void resize(v* v) {
    v->data = realloc(v->data, 2 * sizeof(int)*(v->size));
    v->size = 2 * (v->size);
}

int size(v* v) {
    return v->size;
}

void push(v* v, int val, int index) {
    if (index + 1 >= v->size) {
        resize(v);
    }
    v->data[index] = val;
}

void create(v* v, int size) {
    v->size = size;
    v->data = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        v->data[i] = 0;
    }
}

int read(v* v, int index) {

    if (index + 1 > v->size) {
        printf("out of range \n");
        exit(1);
    }
    return v->data[index];
}

void destroy(v* v) {
    free(v->data);
    free(v);
}

void print(v* v, int m) {
    for (int i = 0; i < m; i++) {
        printf("%d ", v->data[i]);
    }
    printf("\n");
}


