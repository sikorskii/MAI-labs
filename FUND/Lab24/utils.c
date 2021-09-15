//
// Created by djent on 13.04.2021.
//

#include "utils.h"
#include <string.h>
int isNegative = 0;
int numbers = 0;
bool isLetter(unsigned value) {
    return value & (1U << 31);
}

bool isOperator(unsigned value) {
    return (value == '+' || value == '-' || value == '*' || value == '/');
}

bool fill(struct stack* operators, struct stack* exitStack) {
    bool haveDiv = 0;
    char input[100];
    memset(input, 0, sizeof(input));
    int inCnt = 0;
    char ch;
    char *string;
    string = (char*)malloc(sizeof(char));
    int cnt = 0;
    while (true) {
        ch = getchar();
        if (ch == 'X') {
            if (cnt != 0) {
                int toExit = atoi(string);
                free(string);
                string = (char *) malloc(sizeof(char));
                numbers++;
                cnt = 0;
                push(exitStack, toExit);
            }
            break;
        }
        input[inCnt] = ch;
        inCnt++;
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (cnt != 0) {
                int toExit = strtol(string, NULL, 10);
                free(string);
                string = (char*)malloc(sizeof(char));
                cnt = 0;
                numbers++;
                push(exitStack, toExit);
            }
            if (ch == '+' || ch == '-') {
                if (ch == '-') {
                    isNegative++;
                    continue;
                }
                if (!isEmpty(operators)) {
                    while (top(operators) == '*' || top(operators) == '/') {
                        push(exitStack, top(operators));
                        pop(operators);
                    }
                     push(operators, ch);
                }
                else {
                    push(operators, ch);
                }
            }
            else {
                if (ch == '/') {
                    haveDiv = 1;
                }
                push(operators, ch);
            }
        }
        if (ch == '(' || ch == ')') {
            if (cnt != 0) {
                int toExit = atoi(string);
                free(string);
                string = (char *) malloc(sizeof(char));
                cnt = 0;
                numbers++;
                push(exitStack, toExit);
            }
            if (ch == '(') {
                push(operators, ch);
            }
            else {
                if (operators->size == 0)
                    continue;
                while (top(operators) != '(') {
                    push(exitStack, top(operators));
                    pop(operators);
                }
                pop(operators);
            }
        }
        if (ch >= 'A' && ch <= 'z') {
            unsigned a = 1;
            unsigned value = ch | (1U << 31);
            push(exitStack, value);
        }
        if (ch >= '0' && ch <= '9') {
            string[cnt] = ch;
            string = (char*)realloc(string, cnt + 1);
            cnt++;
        }
    }
    while (!isEmpty(operators)) {
        push(exitStack, top(operators));
        pop(operators);
    }
    if (!haveDiv) {
        printf("wrong input %s\n", input);
        return 0;
    }
    bool isOpenFirst = false, isCloseFirst = false;
    int beforeDivCnt = 0, afterDivCnt = 0;
    int alphas = 0, opers = 0;
    int trueMinuses = 0;
    if (input[0] == '-' && !isOperator(input[1])) {
        trueMinuses++;
    }
    if (input[0] == '(') {
        isOpenFirst = true;
    }
    if (inCnt < 3) {
        printf("wrong input %s", input);
        return 0;
    }
    bool isOpenBack = 0, isCloseBack = 0;
    for (int i = 0; i < inCnt; i++) {
        if (input[i] != '(' && input[i] != ')' && input[i] != '/' && input[i] != '-') {
            afterDivCnt++;
        }
        if (input[i] == '/') {
            beforeDivCnt = afterDivCnt;
        }
        if (input[i] <= 'z' && input[i] >= 'A') {
            alphas++;
        }
        if (isOperator(input[i])) {
            opers++;
        }
        if (input[i] == '*' && input[i+1] == '-') {
            trueMinuses++;
        }
        if ((input[i] == '(' || input[i] == ')' || input[i] == '/') && input[i+1] == '-') {
            trueMinuses++;
        }
        if (input[i] == '(' && input[i+1] == ')') {
            printf("wrong input %s\n", input);
            return 0;
        }
        if (input[i] == '+' || (input[i] == '-' && isOperator(input[i+1]))) {
            printf("wrong input %s\n", input);
            return 0;
        }

        if (input[i] == ')' && input[i+1] == '/') {
            isCloseFirst = true;
        }
        if (input[i] == '/' && input[i+1] == '(') {
            isOpenBack = true;
        }
        if ( inCnt > 2 && input[inCnt - 2] == ')') {
            isCloseBack = true;
        }

    }
    opers -= trueMinuses;
    afterDivCnt = alphas + numbers + opers - beforeDivCnt - 1;
    //printf("Brackets : %d %d\n", isOpenFirst, isCloseFirst);
    //printf("Before %d ", beforeDivCnt);
    //printf("After %d\n", afterDivCnt);
    //printf("%d %d %d %d \n", alphas, numbers, opers, trueMinuses);
    if ((alphas + numbers - opers) != 1 ) {
        printf("wrong input %s\n", input);
        return 0;
    }
    if (beforeDivCnt > 1 && (!isOpenFirst || !isCloseFirst)) {
        printf("wrong input %s\n", input);
        return 0;
    }
    if ((isOpenBack && !isCloseBack) && (!isOpenBack && isCloseBack)) {
        printf("wrong input %s\n", input);
        return 0;
    }
    if (afterDivCnt > 1 && !(isOpenBack && isCloseBack)) {
        //printf("%d %d %d", afterDivCnt, isOpenBack, isCloseBack);
        printf("wrong input %s\n", input);
        return 0;
    }
    return 1;
}

void binaryPrinter(unsigned value) {
    for (int i = 31; i >= 0; i--) {
        unsigned mask = 1U << i;
        unsigned result = (mask & value) >> i;
        printf("%u", result);
    }
    printf("\n");
}

void printSign() {
    if (isNegative % 2 == 1)
        printf("-");
}




