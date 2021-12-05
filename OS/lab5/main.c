//
// Created by aldes on 28.11.2021.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>

double (*integrateSin)(double leftBound, double rightBound, double step) = NULL;
double (*calculatePI)(int seriesLength) = NULL;
void *lib = NULL;
bool firstLib = true;
char* path1 = "libFirst.so";
char* path2 = "libSecond.so";

void openLib(char* pathToLib) {
    if (lib != NULL) {
        dlclose(lib);
    }

    lib = dlopen(pathToLib, RTLD_LAZY);
    if (lib == NULL) {
        perror("cant open lib\n");
        exit(1);
    }
    integrateSin = dlsym(lib, "integrateSin");
    calculatePI = dlsym(lib, "calculatePI");
    if (integrateSin == NULL || calculatePI == NULL) {
        perror("unable to find method\n");
        exit(1);
    }
}

void reloadLib() {
    firstLib ? openLib(path2) : openLib(path1);
    firstLib = !firstLib;
    printf("lib changed\n");
}

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
    }

    if (typeOfQuery == 0) {
        reloadLib();
        return 0;
    }
    else {
        printf("invalid query, 0 returned\n");
        return 0;
    }
}


int main() {
    char queryBuf[100];
    openLib(path1);
    printf("enter query:\n");
    while (fgets(queryBuf, 100, stdin) != NULL) {
        printf("calculated %lf\n", parseAndPerform(queryBuf));
        printf("enter query:\n");
    }
    firstLib ? dlclose(path1) : dlclose(path2);
}

