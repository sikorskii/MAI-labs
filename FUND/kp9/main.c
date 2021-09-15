//
// Created by djent on 17.05.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "list.h"

void pars(char* data, struct List* l) {
    char* elem = strtok(data, " +-i");
    long long tempReal = strtol(elem, NULL, 10);
    elem = strtok (NULL, " i");
    char* temp = strtok (NULL, " i");
    long long tempImg;

    if (temp) {
        strcat(elem, temp);
        tempImg = strtol(elem, NULL, 10);
        elem = strtok(NULL, "");
    }
    else
        tempImg = 0;
    char string[100];
    strcpy(string, elem);
    //printf("key: %lld\n", tempReal);
    //printf("%lld\n", tempImg);
    //printf("value: %s\n", string);
    struct data toList;
    toList.key.real = tempReal;
    toList.key.img = tempImg;
    strcpy(toList.data, string);
    iterator last = Last(l);
    Insert(l, &last, toList);
}

bool compare(iterator* lhs, iterator* rhs) {

}

int main(int argc, char *argv[]) {
    struct List l;
    Create(&l);

    FILE *fp;
    char tempo[100];

    if ((fp = fopen(argv[1], "r"))==NULL) {
        printf("Cannot open file.\n");
        exit (1);
    }
    //MAX DATA ARRAY SIZE = 100
    while(!feof(fp)) {
        if (fgets(tempo, 100, fp)) {
            pars(tempo, &l);
        }
    }

    char ch;
    bool sorted = false;
    while ((ch = getchar()) != EOF) {
        switch(ch) {
            case '+': {
                getchar();
                iterator last = Last(&l);
                struct data elem;
                scanf("%lld %lld %s", &elem.key.real, &elem.key.img, &elem.data);
                //getchar();
                Insert(&l, &last, elem);
                sorted = false;
                break;
            }
            case 's': {
                printf("size  %d\n", size(&l));
                break;
            }
            case 'p': {
                if (!sorted)
                    printf("Unsorted:::::::::::::::::::::::::\n\n");
                else printf("Sorted:::::::::::::::::::::::::::\n\n");
                iterator first = First(&l);
                iterator last = Last(&l);
                while(!Equals(&first, &last)) {
                    struct data elem = Read(&first);
                    printf("real: %lld img: %lld data: %s\n", elem.key.real, elem.key.img, elem.data);
                    Next(&first);
                }
                printf("\n\n");
                break;

            }
            case 'i': {
                if (sorted) {
                    printf("already sorted\n\n");
                    break;
                }
                insertionSort(&l);
                sorted = true;
                break;
            }
            case 'b': {
                getchar();
                if (!sorted) {
                    printf("sort the list pls\n\n");
                    break;
                }
                complex key;
                scanf("%lld %lld", &key.real, &key.img);
                iterator first = First(&l);
                iterator last = Last(&l);
                iterator found = binSearch(&l, first, last, key);
                struct data elem = Read(&found);
                if (elem.key.real == key.real && elem.key.img == key.img)
                    printf("FOUND BINARY real: %lld img: %lld data: %s\n", elem.key.real, elem.key.img, elem.data);
                else printf("not in list\n\n");
                break;
            }
            case 'l': {
                getchar();
                if (sorted) {
                    printf("use binary search instead of this crap\n\n");
                    break;
                    }
                complex key;
                scanf("%lld %lld", &key.real, &key.img);
                iterator found = linSearch(&l, &key);
                struct data elem = Read(&found);
                if (elem.key.real == key.real && elem.key.img == key.img)
                    printf("FOUND BINARY real: %lld img: %lld data: %s\n", elem.key.real, elem.key.img, elem.data);
                else printf("not in list\n\n");
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
    //printf("\n\nSorted::::::::::::::\n");
}