//
// Created by djent on 19.04.2021.
//

#include "queue.h"
const int POOL_SIZE = 100;
const int INF = 10e7;
void Create(struct queue* q) {
    q->first = 0;
    q->size = 0;
}

bool Empty(const struct queue* q) {
    return q->size == 0;
}

int Size (const struct queue* q) {
    return q->size;
}

bool Push(struct queue* q, const int t) {
    if (q->size == POOL_SIZE) {
        return false;
    }
    q->data[(q->first + q->size) % POOL_SIZE] = t;
    q->size++;
    return  true;
}

bool Pop(struct queue* q) {
    if (!q->size)
        return false;
    q->first++;
    q->first %= POOL_SIZE;
    q->size--;
    return true;
}

int Top(const struct queue* q) {
    if (q->size)
        return q->data[q->first];
}


void bubbleSort(struct queue* q) {
    int temporary;
    bool isSorting = true;
    while (isSorting) {
        isSorting = false;
        Push(q, INF);
        while(Top(q) != INF) {
            temporary = Top(q);
            Pop(q);
            if (temporary > Top(q)) {
                while (temporary > Top(q))  {
                    Push(q, Top(q));
                    Pop(q);
                }
                Push(q, temporary);
                isSorting = true;
            }
            else {
                Push(q, temporary);
            }
        }
        Pop(q);
    }
}

void clear(struct queue* q) {
    while (!Empty(q)) {
        Pop(q);
    }
}
