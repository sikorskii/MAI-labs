//
// Created by djent on 09.04.2021.
//

#include "cstack.h"
void create(struct stack* s) {
    s->top = 0;
    s->size = 0;
}

bool isEmpty(struct stack* s) {
    return s->top == 0;
}

int size(struct stack* s) {
    return s->size;
}

bool push(struct stack* s, int value) {
    struct Node* current = (struct Node*)malloc(sizeof(struct Node));
    if (!current)
        return false;
    current->data = value;
    current->prev = s->top;
    s->top = current;
    s->size++;
    return true;
}

bool pop(struct stack* s) {
    if (!s->size)
        return false;
    struct Node* current = s->top;
    s->top = s->top->prev;
    s->size--;
    free(current);
    return true;
}

int top(struct stack* s) {
    if (s->top)
        return s->top->data;
}

void destroy(struct stack* s) {
    while (s->top) {
        struct Node* current = s->top;
        s->top = s->top->prev;
        free(current);
    }
    s->top = 0;
    s->size = 0;
}
