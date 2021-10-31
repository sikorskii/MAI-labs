//
// Created by aldes on 19.09.2021.
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
// 0 - reading
// 1 - writing
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
    printf("\ni am child and i will write in file %s\n", argv[1]);

    int fd[2];
    fd[0] = (int) strtol(argv[2], nullptr, 10);
    fd[1] = (int) strtol(argv[3], nullptr, 10);

    FILE *fp = fopen(argv[1], "w");
    fprintf(fp, "child been here\n");

    close(fd[1]);
    char buf[50];

    while(read(fd[0], buf, sizeof(buf)) != 0) {
        reverse(buf);
        //printf("Recieved : %s\n", buf);
        fprintf(fp, "Received string: %s\n", buf);
        fflush(fp);
    }

    fclose(fp);
    return 0;
}

