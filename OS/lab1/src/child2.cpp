//
// Created by aldes on 19.09.2021.
//

#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[]) {
    printf("\ni am child 2 and i will write in file %s\n", argv[0]);

    int fd[2];
    fd[0] = (int)strtol(argv[1], nullptr,  10);
    fd[1] = (int)strtol(argv[2], nullptr, 10);

    FILE *fp = fopen(argv[0], "w");

    fprintf(fp, "child2 been here\n");
    fprintf(fp, "%d\n", fd[0]);
    fprintf(fp, "%d\n", fd[1]);

    fclose(fp);
    return 0;
}
