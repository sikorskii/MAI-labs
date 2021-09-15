//
// Created by djent on 13.04.2021.
//

#ifndef CTREE24_UTILS_H
#define CTREE24_UTILS_H
#include <stdbool.h>
#include "cstack.h"
#include <stdio.h>
#include <stdlib.h>
bool fill(struct stack* operators, struct stack* exitStack);
void binaryPrinter(unsigned value);
bool isLetter(unsigned top);
bool isOperator(unsigned top);
void printSign();
#endif //CTREE24_UTILS_H
