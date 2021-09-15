//
// Created by djent on 15.05.2021.
//
#ifndef _vector_h
#define _vector_h
typedef struct {
    int* data;
    int size;
} v;

void resize(v* v);

int size(v* v);

void push(v* v, int val, int index);

void create(v* v, int size);

int read(v* v, int index);

void vectorDestroy(v* v);

void print(v* v, int m);
#endif //_vector_h