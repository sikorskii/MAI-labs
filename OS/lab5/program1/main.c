//integrate sin(X) | PI calculation
#include <stdio.h>
#include "../libSrc/src/public/headers/api.h"

int main() {
    char queryBuf[100];
    printf("enter query:\n");
    while (fgets(queryBuf, 100, stdin) != NULL) {
        printf("calculated %lf\n", compute(queryBuf));
        printf("enter query:\n");
    };
}
