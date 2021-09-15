//
// Created by djent on 15.05.2021.
//

#include <stdlib.h>
#include <stdio.h>

#include "list.h"


bool Equals(const iterator* lhs, const iterator* rhs) {
    return lhs->node == rhs->node;
}

iterator Next(iterator* i) {
    i->node = i->node->next;
}

iterator Prev(iterator* i) {
    i->node = i->node->prev;
}

Person Read(const iterator* i) {
    return i->node->person;
}

void Write(const iterator* i, Person data) {
    i->node->person = data;
}

void Create(struct List* l) {
    l->head = (struct Node*) malloc(sizeof(struct Node));
    l->head->next = l->head->prev = l->head;
    l->size = 0;
}

iterator First(const struct List* l) {
    iterator i = {l->head->next};
    return i;
}

iterator Last(const struct List* l) {
    iterator i = {l->head};
    return i;
}

iterator nPos(const struct List* l, int n) {
    n--;
    if (n >= size(l)) {
        printf("NIL, first element returned");
        iterator first = First(l);
        return first;
    }
    iterator iterator = First(l);
    for (int i = 0; i < n; i++) {
        Next(&iterator);
    }
    return iterator;
}

iterator move(iterator iterator, int n) {
    for (int i = 0; i < n; i++)
        Next(&iterator);
}

bool isEmpty(const struct List* l) {
    iterator lhs = First(l);
    iterator rhs = Last(l);
    return Equals(&lhs, &rhs);
}

int size(const struct List* l) {
    return l->size;
}

iterator Insert(struct List* l, iterator* i, Person data) {
    iterator iterator = {malloc(sizeof(struct Node))};
    if (!iterator.node)
        return Last(l);
    iterator.node->person = data;
    iterator.node->next = i->node;
    iterator.node->prev = i->node->prev;
    iterator.node->prev->next = iterator.node;
    i->node->prev = iterator.node;
    l->size++;
    return iterator;
}

iterator Delete(struct List* l, iterator* i) {
    iterator iterator = Last(l);
    if(Equals(i, &iterator))
        return iterator;
    iterator.node = i->node->next;
    iterator.node->prev = i->node->prev;
    i->node->prev->next = iterator.node;
    l->size--;
    free(i->node);
    i->node = 0;
    return iterator;
}

void Destroy(struct List* l) {
    iterator first = First(l);
    iterator last = Last(l);
    while (!Equals(&first, &last)) {
        iterator temp = first;
        Next(&first);
        Delete(l, &temp);
    }
    l->head = NULL;
    l->size = 0;
}
