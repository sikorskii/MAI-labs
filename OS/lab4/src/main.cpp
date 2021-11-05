#include "unistd.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <semaphore.h>
#include <sys/mman.h>


#include "mem.h"

int main() {

    char buf[MAX_LENGTH];

    char *filename1;
    char *filename2;

    printf("Enter file1 name\n");
    //scanf("%s", buf);
    filename1 = fgets (buf, sizeof(buf) - 1, stdin);
    asprintf(&filename1, "%s", buf);

    printf("Enter file2 name\n");
    //scanf("%s", buf);
    filename2 = fgets (buf, sizeof(buf) - 1, stdin);
    asprintf(&filename2, "%s", buf);

    pid_t child1_pid, child2_pid;

    sem_t *firstSem = sem_open(sem1, O_CREAT, 0644, 0);
    sem_t *firstSemOut = sem_open(sem11, O_CREAT, 0644, 0);
    sem_t *secondSem = sem_open(sem2, O_CREAT, 0644, 0);
    sem_t *secondSemOut = sem_open(sem22, O_CREAT, 0644, 0);

    int fd1 = shm_open(file1,
                      O_RDWR | O_CREAT,
                      accessPerms);

    ftruncate(fd1, NUMBER_OF_BYTES);
    if (fd1 < 0)
        handleError("fd1 create error");

    caddr_t memptr1 = static_cast<caddr_t>(mmap(NULL,
                                               NUMBER_OF_BYTES,
                                               PROT_READ | PROT_WRITE,
                                               MAP_SHARED,
                                               fd1,
                                               0));

    if (memptr1 == (caddr_t)-1)
        handleError("memptr1 mapping error");

    int fd2 = shm_open(file2,
                       O_RDWR | O_CREAT,
                       accessPerms);

    ftruncate(fd2, NUMBER_OF_BYTES);
    if (fd2 < 0)
        handleError("fd2 create error");

    caddr_t memptr2 = static_cast<caddr_t>(mmap(NULL,
                                                NUMBER_OF_BYTES,
                                                PROT_READ | PROT_WRITE,
                                                MAP_SHARED,
                                                fd2,
                                                0));
    if (memptr2 == (caddr_t)-1)
        handleError("memptr2 mapping error");


    child1_pid = fork();

    if (child1_pid == -1)
        handleError("fork error");


    else if (child1_pid == 0) { //child1

        printf("[%d] It's child1\n", getpid());
        fflush(stdout);
        execl("child1.out", "child1", filename1, sem1, sem11, file1, NULL); //execution of child1's program begins here

    }

    else { //parent

        printf("[%d] It's parent. Child id: %d\n", getpid(), child1_pid);
        fflush(stdout);

        child2_pid = fork();

        if (child2_pid == -1)
            handleError("fork 2 error");

        else if (child2_pid == 0) { //child2

            printf("[%d] It's child2\n", getpid());
            fflush(stdout);
            execl("child1.out", "child2", filename2, sem2, sem22, file2, NULL); //execution of child2's program begins here

        }

        //parent code below

        free(filename1);
        free(filename2);

        char *str;


        while (true) {
            char c[50];
            str = fgets(c, sizeof(c), stdin);

            if (strlen(c) == 1)
                continue;

            if (str == nullptr)
                break;

            if ((strlen(c) - 1) % 2 == 0) {
                strcpy(memptr1, c);
                //sleep(2);
                printf("Parent opens first sem\n");
                sem_post(firstSem);
                sem_wait(firstSemOut);


            } else {
                strcpy(memptr2, c);
                //sleep(2);
                printf("Parent opens second sem\n");
                sem_post(secondSem);
                sem_wait(secondSemOut);
            }
        }
        char c[2] = "\0";

        strcpy(memptr1, c);
        sem_post(firstSem);
        sem_wait(firstSemOut);

        strcpy(memptr2, c);
        sem_post(secondSem);
        sem_wait(secondSemOut);

        munmap(memptr1, NUMBER_OF_BYTES);
        munmap(memptr2, NUMBER_OF_BYTES);

        close(fd1);
        close(fd2);

        sem_close(firstSem);
        sem_close(firstSemOut);
        sem_close(secondSem);
        sem_close(secondSemOut);

        shm_unlink(file1);
        shm_unlink(file2);
        return 0;
    }
}
