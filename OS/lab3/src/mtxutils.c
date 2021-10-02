//
// Created by aldes on 02.10.2021.
//

#include "mtxutils.h"

int getRandInt() {

}

bool prompt() {
    printf("Fill matrix with random values?\n Enter Y/N \n");
    char mode;
    scanf("%c", &mode);

    switch (mode) {
        case 'Y' :
            return true;
        case 'N' :
            return false;
        default :
            prompt();
    }
    return false;
}

int** getMatrix() {
    int n;
    printf("Enter matrix's dimension: \n");
    scanf("%d", &n);
    if (prompt()) {

    }
    else {

    }
}