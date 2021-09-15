//
// Created by djent on 15.05.2021.
//
#include <stdio.h>
#include <stdlib.h>
#include "list.h"


int main() {
    struct List l;
    Create(&l);
    char ch;
    while ((ch = getchar()) != EOF) {
        if (ch == EOF) {
            struct Node* i = l.head->next;
            while (i != l.head) {
                struct Node* current = l.head;
                i = i->next;
                free(current);
            }
            free(l.head);
            l.head = NULL;
            l.size = 0;
            printf("\n");
        }
        switch(ch) {
            case '+': {
                getchar();
                int value, pos;
                scanf("%d",&value);
                getchar();
                scanf("%d", &pos);
                if (pos == size(&l) + 1) {
                    iterator place = Last(&l);
                    Insert(&l, &place, value);
                    printf("\n");
                    break;
                }
                if (pos > size(&l) || pos == 0) {
                    printf("out of size\n");
                    break;
                }
                iterator place = nPos(&l, pos);
                Insert(&l, &place, value);
                printf("\n");
                break;
            }
            case '-': {
                getchar();
                int pos;
                scanf("%d", &pos);
                if (pos > size(&l) || pos == 0) {
                    printf("out of size\n");
                    break;
                }
                iterator place = nPos(&l, pos);
                int val = Read(&place);
                Delete(&l, &place);
                printf("element %d at position %d deleted\n", val, pos);
                break;
            }
            case 'r': {
                getchar();
                int pos;
                scanf("%d", &pos);
                iterator iterator1 = nPos(&l, pos);
                printf("%d ", Read(&iterator1));
                printf("\n");
                break;
            }
            case 's': {
                printf("size  %d\n", size(&l));
                break;
            }
            case 'p': {
                iterator i;
                iterator last = Last(&l);

                for (i = First(&l); !Equals(&i, &last); Next(&i)) {
                    printf("%d ", Read(&i));
                }
                printf("\n");
                break;

            }
            case 'a': {
                getchar();
                int k;
                scanf("%d",&k);
                int index = 0;
                iterator last = Last(&l);
                iterator iterator1 = First(&l);
                for (iterator1 = First(&l); !Equals(&iterator1, &last); ){
                    index++;
                    if (index % k == 0) {
                        iterator temp = iterator1;
                        Delete(&l, &iterator1);
                        iterator1 = temp;
                    }
                    Next(&iterator1);
                }
                printf("\n");
                break;
            }
            case 'c': {
                int k = 1;
                int index = 0;
                iterator last = Last(&l);
                iterator iterator1 = First(&l);
                for (iterator1 = First(&l); !Equals(&iterator1, &last); ){
                    index++;
                    if (index % k == 0) {
                        iterator temp = iterator1;
                        Delete(&l, &iterator1);
                        iterator1 = temp;
                    }
                    Next(&iterator1);
                }
                printf("\n");
                l.size = 0;
                break;
            }
            case 'X': {
                int k = 1;
                int index = 0;
                iterator last = Last(&l);
                iterator iterator1 = First(&l);
                for (iterator1 = First(&l); !Equals(&iterator1, &last); ){
                    index++;
                    if (index % k == 0) {
                        iterator temp = iterator1;
                        Delete(&l, &iterator1);
                        iterator1 = temp;
                    }
                    Next(&iterator1);
                }
                printf("\n");
                l.head = NULL;
                l.size = 0;
                return 0;
            }

        }

    }
}
