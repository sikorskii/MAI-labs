//
// Created by aldes on 28.11.2021.
//

#include <stdio.h>
#include <string.h>
#include "lib1.h"

double parseAndPerform(char* query) {
    int typeOfQuery;
    char* type = strtok(query, " ");
    sscanf(type, "%d", &typeOfQuery);

    if (typeOfQuery == 1) {
        char* firstArg = strtok(NULL, " ");
        if (firstArg == NULL)
            return 0;
        double first;
        sscanf(firstArg, "%lf", &first);
        char* secondArg = strtok(NULL, " ");
        if (secondArg == NULL)
            return 0;
        double second;
        sscanf(secondArg, "%lf", &second);
        char* thirdArg = strtok(NULL, " ");
        if (thirdArg == NULL)
            return 0;
        double third;
        sscanf(thirdArg, "%lf", &third);
        while (thirdArg != NULL) {
            thirdArg = strtok(NULL, " \n\0");
        }
        printf("first arg %lf second %lf third %lf\n", first, second, third);
        return integrateSin(first, second, third);
    }

    if (typeOfQuery == 2) {
        char* firstArg = strtok(NULL, " ");
        if (firstArg == NULL)
            return 0;
        //printf("first arg is %s\n", firstArg);
        int first;
        sscanf(firstArg, "%d", &first);
        while (firstArg != NULL) {
            firstArg = strtok(NULL, " \n\0");
        }
        //printf("first arg %d\n", first);
        printf("stat function begin\n");
        return calculatePI(first);
    }
    else {
        printf("invalid query, 0 returned\n");
        return 0;
    }

}

int main() {
    char queryBuf[100];
    printf("enter query:\n");
    while (fgets(queryBuf, 100, stdin) != NULL) {
        printf("calculated %lf\n", parseAndPerform(queryBuf));
        printf("enter query:\n");
    }
}

