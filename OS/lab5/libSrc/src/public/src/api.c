//
// Created by aldes on 21.11.2021.
//

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/api.h"
#include "../../private/headers/operations.h"

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
        return calculatePI(first);
    } else {
        printf("invalid query, 0 returned\n");
        return 0;
    }

}


double compute(char* query) {
    return parseAndPerform(query);
}
