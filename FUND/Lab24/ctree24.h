//
// Created by djent on 09.04.2021.
//

#ifndef CTREE24_CTREE24_H
#define CTREE24_CTREE24_H
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

struct treeNode {
    unsigned value;
    struct treeNode* lTree;
    struct treeNode* rTree;
    struct treeNode* parent;
};

struct tree {
    struct treeNode* root;
};

void printEl (unsigned value);
void editedTree(struct treeNode* current, unsigned *output, int *oCnt);
void insert(struct tree*, struct stack*);
void fillLnR(struct treeNode* current, struct treeNode* tree, int* cnt);
void createTree(struct tree* t);
bool isLeaf(struct treeNode* current);
void printTree(struct treeNode* current);
void unchain(struct treeNode* current);
void execute(struct tree* t, struct treeNode* current);
void print(struct treeNode* current, struct tree* t);
void printFinal(struct treeNode* current, struct tree *t);
void printOne();
void nclear(struct tree* t);
void clear(struct tree*, struct treeNode*);
#endif //CTREE24_CTREE24_H
