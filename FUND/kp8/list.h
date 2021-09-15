//
// Created by djent on 15.05.2021.
//

#ifndef KP8_LIST_H
#define KP8_LIST_H

#include <stdbool.h>

struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

struct List {
    struct Node* head;
    int size;
};

typedef struct {
    struct Node* node;
} iterator;

bool Equals(const iterator* lhs, const iterator* rhs);

iterator Next(iterator* i);

iterator Prev(iterator* i);

iterator nPos(const struct List* l, int n);

iterator move(const struct List* l, iterator iterator, int n);

int Read(const iterator* i);

void Write(const iterator* i, const int data);

void Create(struct List* l);

iterator First(const struct List* l);

iterator Last(const struct List* l);

bool isEmpty(const struct List* l);

int size(const struct List* l);

iterator Insert(struct List* l, iterator* i, const int data);

iterator Delete(struct List* l, iterator* i);

void Destroy(struct List* l);
#endif //KP8_LIST_H
