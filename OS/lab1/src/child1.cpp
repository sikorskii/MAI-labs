//
// Created by aldes on 19.09.2021.
//

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
// 0 - reading
// 1 - writing
int main(int argc, char* argv[]) {
    printf("\ni am child 1 and i will write in file %s\n", argv[0]);

    int fd[2];
    fd[0] = (int)strtol(argv[1], nullptr,  10);
    fd[1] = (int)strtol(argv[2], nullptr, 10);

    FILE *fp = fopen(argv[0], "w");

    fprintf(fp, "child1 been here\n");
    fprintf(fp, "%d\n", fd[0]);
    fprintf(fp, "%d\n", fd[1]);
    fflush(stdout);

    int x, y;
    char buf[50];

    read(fd[0], &x, sizeof(int));
    read(fd[0], &y, sizeof(int));
    fflush(stdout);
    fprintf(fp, "RESULT IS %d\n", x + y);

    close(fd[0]);
    close(fd[1]);
    fclose(fp);
    return 0;
}

