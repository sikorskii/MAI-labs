//
// Created by aldes on 19.09.2021.
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "mem.h"

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


    FILE *fp = fopen(argv[1], "w");
    fprintf(fp, "child been here\n");

    char buf[50];

    sem_t *sem = sem_open(argv[2], 0);
    sem_t *semOut = sem_open(argv[3], 0);

    int fd = shm_open(argv[4], O_RDWR, accessPerms);
    if (fd < 0)
        handleError("child fd error");

    char* memptr = static_cast<char*>(mmap(NULL,
                                               NUMBER_OF_BYTES,
                                               PROT_READ | PROT_WRITE,
                                               MAP_SHARED,
                                               fd,
                                               0));
    if (memptr == (char*)-1)
        handleError("child memptr error");

    while (true) {
        if(!sem_wait(sem)) {
            printf("%d sem opened!\n", getpid());

            if (strcmp(memptr, "\0") == 0)
                break;

            reverse(memptr);
            printf("reversed: %s\n", memptr);
            fprintf(fp, "Got and reversed from shared memory:");
            fprintf(fp, memptr);
            fprintf(fp, "\n");
            printf("%d inside\n", getpid());
            sem_post(semOut);
        }
    }
    printf("%d is about to end his work\n", getpid());
    sem_post(semOut);

    munmap(memptr, NUMBER_OF_BYTES);
    close(fd);
    sem_close(sem);
    sem_close(semOut);
    unlink(argv[4]);

    fclose(fp);
    return 0;
}

