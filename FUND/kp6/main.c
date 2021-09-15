//
// Created by djent on 24.05.2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int counter(iterator* first, iterator* last, unsigned number) {
    int badBoys = 0;
    while (!Equals(first, last)) {
        //printf("%s\n", Read(first).name);
        bool isStipuha = true;
        Person p = Read(first);

        if (!(p.mark1 > 3 && p.mark2 > 3 && p.mark3 > 3 && p.mark4 > 3))
            isStipuha = false;

        if (isStipuha && p.group == number && !strcmp(p.sex, "f"))
            badBoys++;

        Next(first);
    }
    return badBoys;
}

int main(int argc, char *argv[]) {

    if (argc == 1) {
        printf("few args\n");
        printf("input.txt -p n, where n is group number\n");
        exit(1);
    }

    if (argc == 2) {
        printf("few args\n");
        printf("input.txt -p n, where n is group number\n");
        exit(1);
    }

    int cmp = strcmp(argv[2], "-p");
    int cmp1 = strcmp(argv[2], "-f");

    if (cmp != 0 && cmp1) {
        printf("wrong args, use -p n , where n is group number");
        exit(1);
    }

    int number = abs(strtol(argv[3], NULL, 10));
    bool isPrint = false;

    if (cmp1 == 0) {
        printf("Printing file...\n");
        isPrint = true;
    }
    else {

        if (!(number >= 106 && number <= 108)) {
            printf("wrong args. possible args is -106, -107, -108");
            exit(1);
        }
    }

    FILE *fp;

    if ((fp = fopen(argv[1], "r")) == NULL) {
        printf("Cannot open file.\n");
        exit (1);
    }

    Person p;
    struct List l;
    Create(&l);
    iterator last = Last(&l);

    while(!feof(fp)) {
        fscanf(fp, "%s %s %s %u %u %u %u %u", p.name, p.surname, p.sex, &p.group, &p.mark1, &p.mark2, &p.mark3, &p.mark4);
        Insert(&l, &last, p);
        last = Last(&l);
    }

    iterator first = First(&l);

    if (isPrint) {
        while (!Equals(&first, &last)) {
            Person toPrint = Read(&first);
            printf("%s %s %s %u %u %u %u %u\n", toPrint.name, toPrint.surname, toPrint.sex, toPrint.group, toPrint.mark1,
                   toPrint.mark2, toPrint.mark3, toPrint.mark4);
            Next(&first);
        }
    }

    fclose(fp);

    if (!isPrint) {
        //printf("%s\n", argv[2]);
        printf("group:  %d\n", number);
        first = First(&l);
        last = Last(&l);
        int badBoys = counter(&first, &last, number);
        printf("%d\n", badBoys);
    }

    Destroy(&l);
}
