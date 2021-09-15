//
// Created by djent on 09.04.2021.
//
#include "ctree24.h"
int eCnt = 0;
int INF = 1000000000+7;
bool first = 1;
bool upOne = true;
bool downOne = true;
bool isLeaf(struct treeNode* current) {
    return (!isOperator(current->value) && !isLetter(current->value)) || isLetter(current->value);
}

void createTree(struct tree* t) {
    t->root = NULL;
}

void subInsert(struct treeNode* current, struct stack* s) {
    if (size(s) == 0) {
        return;
    }
    if (isLeaf(current)) {
        current = current->parent;
        while(current->lTree) {
            if (!current->rTree)
                break;
            current = current->parent;
        }
        current->rTree = (struct treeNode*)malloc(sizeof(struct treeNode));
        current->rTree->parent = current;
        current = current->rTree;
        current->value = top(s);
        pop(s);
        current->lTree = current->rTree = NULL;
        subInsert(current, s);
    }
    if (!current->lTree && !isLeaf(current)) {
        current->lTree = (struct treeNode*)malloc(sizeof(struct treeNode));
        current->lTree->parent = current;
        current = current->lTree;
        current->value = top(s);
        pop(s);
        current->lTree = current->rTree = NULL;
        subInsert(current, s);
    } else {
        if (!current->rTree && !isLeaf(current)) {
            current->rTree = (struct treeNode*)malloc(sizeof(struct treeNode));
            current->rTree->parent = current;
            current = current->rTree;
            current->value = top(s);
            pop(s);
            current->lTree = current->rTree = NULL;
            subInsert(current, s);
        }
    }
}

void insert(struct tree* t, struct stack* s) {
        if (s->size == 0)
            return;
        if (!t->root) {
            t->root = (struct treeNode*)malloc(sizeof(struct treeNode));
            t->root->value = top(s);
            pop(s);
            t->root->parent = NULL;
            t->root->lTree = NULL;
            t->root->rTree = NULL;
            subInsert(t->root, s);
            return;
        }
}

void printEl (unsigned value) {
    if(isLetter(value) || isOperator(value)) {
        unsigned a = value << 1;
        a = a >> 1;
        printf("%c", a);
        return;
    }
    else {
        printf("%u", value);
        return;
    }
}

void printTree(struct treeNode* current) {
    if (!current->lTree && !current->rTree) {
        printEl(current->value);
        return;
    }
    if (current->lTree) {
        printTree(current->lTree);
    }
    printEl(current->value);
    if (current->rTree) {
        printTree(current->rTree);
    }
}

void rightPath(struct treeNode* current, struct treeNode* comp) {
    if (!current->rTree && !current->lTree) {
        if (current->value == comp->value && current->value != INF && comp->value != INF) {
            current->value = comp->value = INF;
        }
        else return;
    }
    if (current->lTree) {
        rightPath(current->lTree, comp);
    }
    if (current->rTree) {
        rightPath(current->rTree, comp);
    }
}


void execute(struct tree* t, struct treeNode* current) {
    if (!current->lTree && !current->rTree) {
        rightPath(t->root->rTree, current);
    }
    if (current->lTree) {
        execute(t, current->lTree);
    }
    if (current->rTree) {
        execute(t, current->rTree);
    }
}

void printFinal(struct treeNode* current, struct tree *t) {
    if (!current->lTree && !current->rTree) {
        if (current->value != INF) {
            if (!first)
                printf("%c", '*');
            printEl(current->value);
            upOne = false;
            downOne = true;
            first = 0;
        }
        return;
    }
    if (current->rTree) {
        printFinal(current->rTree, t);
    }
    if (current->value == '/') {
        if (upOne)
            printf("1");
        printf("%c", ')');
        printEl(current->value);
        printf("%c", '(');
        first = 1;
        downOne = false;
    }
    if (current->lTree) {
        printFinal(current->lTree, t);
    }
}

void printOne() {
    if (!downOne)
        printf("1");
}


void delete(struct tree* t, struct treeNode* current) {
    if (current->parent->lTree) {
        if (current->parent->lTree == current) {
            current->parent->lTree = NULL;
            free(current);
            return;
        }
    }
    if (current->parent->rTree) {
        if (current->parent->rTree == current) {
            current->parent->rTree = NULL;
            free(current);
            return;
        }
    }
}

void nclear(struct tree* t) {
    while (t->root) {
        clear(t, t->root);
    }
}
void clear(struct tree*t, struct treeNode* current) {
    if (!current) {
        return;
    }
    if (!current->lTree && !current->rTree) {
        if (current == t->root) {
            if (current->rTree) {
                t->root = current->rTree;
            }
            else  {
                t->root = NULL;
                return;
            }
        }
        delete(t, current);
        return;
    }
    if (current->lTree) {
        clear(t, current->lTree);
    }
    if (current->rTree) {
        clear(t, current->rTree);
    }
}
