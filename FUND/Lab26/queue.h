//
// Created by djent on 19.04.2021.
//

#ifndef LAB26_QUEUE_H
#define LAB26_QUEUE_H

#include <stdbool.h>

struct queue {
    int first;
    int size;
    int data[100];
};

void Create(struct queue* q);

bool Empty(const struct queue* q);

int Size (const struct queue* q);

bool Push(struct queue* q, const int t);

bool Pop(struct queue* q);

int Top(const struct queue* q);

void bubbleSort(struct queue* q);

void clear(struct queue* q);
#endif //LAB26_QUEUE_H
