//
// Created by djent on 15.05.2021.
//

#ifndef KP8_LIST_H
#define KP8_LIST_H

#include <stdbool.h>

typedef struct {
    long long real;
    long long img;
} complex;

struct data {
    complex key;
    char data[100];
};

struct Node {
    struct data info;
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

struct data Read(const iterator* i);

complex ReadCmp(const iterator* i);

void Write(const iterator* i, struct data elem);

void Create(struct List* l);

iterator First(const struct List* l);

iterator Last(const struct List* l);

bool isEmpty(const struct List* l);

int size(const struct List* l);

iterator Insert(struct List* l, iterator* i, struct data elem);

iterator Delete(struct List* l, iterator* i);

void Destroy(struct List* l);

int distance(iterator lhs, iterator rhs);

iterator binSearch(struct List* l, iterator lhs, iterator rhs, complex value);

iterator linSearch(struct List* l, complex* key);

void insertionSort(struct List* l);
#endif //KP8_LIST_H
