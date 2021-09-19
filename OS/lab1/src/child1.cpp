//
// Created by aldes on 19.09.2021.
//

#include <cstdio>

int main(int argc, char* argv[]) {
    printf("\ni am child 1 and i will write in file %s\n", argv[0]);
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    FILE *fp = fopen(argv[0], "w");
    fprintf(fp, "child1 been here\n");
    fclose(fp);
    return 0;
}

