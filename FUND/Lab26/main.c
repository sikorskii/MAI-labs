//
// Created by djent on 19.04.2021.
//

#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
void pars(struct queue* q);
signed main() {
    //MAX Q.SIZE is 100
    struct queue q;
    pars(&q);
}

void pars(struct queue* q) {
    Create(q);
    char ch;
    int value;
    while(true) {
        ch = getchar();
        if (ch == EOF) {
            return;
        }
        getchar();
        if (ch != 'p' && ch != 'c' && ch != 's' && ch != '-' && ch != 'o') {
            scanf("%d", &value);
            getchar();
        };
        switch (ch) {
            case '+': {
                Push(q ,value);
                break;
            }
            case 'c': {
                clear(q);
                break;
            }
            case '-': {
                Pop(q);
                printf("\n");
                break;
            }
            case 's': {
                bubbleSort(q);
                break;
            }

            case 'p': {
                printf("Size of queue is %d\n", Size(q));
                break;
            }

            case 'o': {
                while (!Empty(q)) {
                    printf("%d ", Top(q));
                    Pop(q);
                }
                printf("\n");
            }
            default: continue;
        }
    }
}