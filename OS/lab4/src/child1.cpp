//
// Created by aldes on 19.09.2021.
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#include "mem.h"

void reverse(char *str) {
    int i = 0;
    int j = (int)strlen(str) - 1;

    while(i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int main(int argc, char* argv[]) {

    const int MAX_STRING_LENTGH = 50;

    printf("\ni am child and i will write in file %s\n", argv[1]);

    int fileDescriptor = (int) strtol(argv[2], nullptr, 10);

    FILE *fp = fopen(argv[1], "w");
    fprintf(fp, "child been here\n");
    fprintf(fp, "my descriptor is %d\n", fileDescriptor);

    char buf[MAX_STRING_LENTGH] = "cringe";

    while(true /*read from shared here*/) {
        reverse(buf);
        //printf("Recieved : %s\n", buf);
        fprintf(fp, "Received string: %s\n", buf);
        fflush(fp);
        break; //uncomment this
    }

    fclose(fp);
    return 0;
}

