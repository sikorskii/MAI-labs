//
// Created by djent on 09.04.2021.
//

#ifndef CTREE24_CSTACK_H
#define CTREE24_CSTACK_H
#include <stdbool.h>
#include <malloc.h>
struct Node {
    unsigned data;
    struct Node* prev;
};

struct stack {
    struct Node* top;
    int size;
};

void create(struct stack* s);

bool isEmpty(struct stack* s);

int size(struct stack* s);

bool push(struct stack* s, int value);

bool pop(struct stack* s);

int top(struct stack* s);

void destroy(struct stack* s);
#endif //CTREE24_CSTACK_H
