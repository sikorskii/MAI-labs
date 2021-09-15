//
// Created by djent on 15.05.2021.
//

#include <stdlib.h>
#include <string.h>
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

struct data Read(const iterator* i) {
    return i->node->info;
}

void Write(const iterator* i, struct data elem) {
    i->node->info = elem;
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

bool isEmpty(const struct List* l) {
    iterator lhs = First(l);
    iterator rhs = Last(l);
    return Equals(&lhs, &rhs);
}

int size(const struct List* l) {
    return l->size;
}

iterator Insert(struct List* l, iterator* i, struct data elem) {
    iterator iterator1 = {malloc(sizeof(struct Node))};
    if (!iterator1.node)
        return Last(l);
    iterator1.node->info = elem;
    iterator1.node->next = i->node;
    iterator1.node->prev = i->node->prev;
    iterator1.node->prev->next = iterator1.node;
    i->node->prev = iterator1.node;
    l->size++;
    return iterator1;
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
    struct Node* i = l->head->next;
    while(i != l->head) {
        struct Node* current = l->head;
        i = i->next;
        free(current);
    }
    free(l->head);
    l->head = 0;
    l->size = 0;
}

iterator linSearch(struct List* l, complex* key) {
    iterator first = First(l);
    iterator last = Last(l);

    while (!Equals(&first, &last)) {

        if (Read(&first).key.real == key->real && Read(&first).key.img == key->img)
            return first;

        Next(&first);

    }
    return first;
}

bool cmp(complex* lhs, complex* rhs) {
    long long lval = lhs->real * lhs->real + lhs->img * lhs->img;
    long long rval = rhs->real * rhs->real + rhs->img * rhs->img;
    return lval > rval;
}

bool cmp2(complex* lhs, complex* rhs) {
    long long lval = lhs->real * lhs->real + lhs->img * lhs->img;
    long long rval = rhs->real * rhs->real + rhs->img * rhs->img;
    return lval >= rval;
}

int distance(iterator lhs, iterator rhs) {
    int cnt = 0;
    while (!Equals(&lhs, &rhs)) {
        Next(&lhs);
        cnt++;
    }
    return cnt;
}

iterator nPos(const struct List* l, int n) {
    iterator iterator = First(l);
    for (int i = 0; i < n; i++) {
        Next(&iterator);
    }
    return iterator;
}

iterator binSearch(struct List* l, iterator lhs, iterator rhs, complex value) {
    //Prev(&rhs);
    iterator first = First(l);
    while (distance(lhs, rhs) > 1) {
        int m = (distance(first, lhs) + distance(first, rhs)) / 2;
        iterator mid = nPos(l, m);
        complex key = Read(&mid).key;
        if (cmp2(&value, &key)) {
            lhs = mid;
        }
        else rhs = mid;
    }
    return lhs;
}

void sortInsert(struct List* l, struct data unit) {
    iterator first = First(l);
    iterator last = Last(l);
    //Next(&first);
    while (!Equals(&first, &last)) {
        struct data cmpUnit = Read(&first);
        if (!cmp(&unit.key, &cmpUnit.key)) {
            Insert(l, &first, unit);
            return;
        }
        Next(&first);
    }
    Insert(l, &last, unit);
}

void insertionSort(struct List* l) {
    bool unsorted = true;
    while (unsorted) {
        unsorted = false;
        iterator first = First(l);
        iterator last = Last(l);
        Prev(&last);
        while (!Equals(&first, &last)) {
            complex lhs = Read(&first).key;
            Next(&first);
            complex rhs = Read(&first).key;
            if (cmp(&lhs, &rhs)) {
                unsorted = true;
                struct data unit = Read(&first);
                Delete(l, &first);
                sortInsert(l, unit);
                break;
            }
        }
    }
}
