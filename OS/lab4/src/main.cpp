#include "unistd.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/mman.h>
#include <semaphore.h>

#include "mem.h"

// 0 - reading
// 1 - writing
//MAX STRING LENGTH IS 50

int main() {

    const int MAX_STRING_LENTGH = 50;

    int mapDescriptor1 = shm_open(file1, O_RDWR | O_CREAT, accessPerms);
    if (mapDescriptor1 < 0) {
        printf("cant open shared memory file");
        exit(1);
    }

    int mapDescriptor2 = shm_open(file2, O_RDWR | O_CREAT, accessPerms);
    if (mapDescriptor2 < 0) {
        printf("cant open shared memory file");
        exit(1);
    }

    ftruncate(mapDescriptor1, MAX_STRING_LENTGH * sizeof(char));
    ftruncate(mapDescriptor2, MAX_STRING_LENTGH * sizeof(char));

    printf("map1: %d, map2: %d\n", mapDescriptor1, mapDescriptor2);

    char buf[MAX_STRING_LENTGH];

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

    char *toc1;
    asprintf(&toc1, "%d", mapDescriptor1);


    child1_pid = fork();

    if (child1_pid == -1) {
        printf("fork error!\n");
    }

    else if (child1_pid == 0) { //child1

        printf("[%d] It's child1\n", getpid());
        fflush(stdout);
        execl("child1.out", "child1", filename1, toc1, NULL); //execution of child1's program begins here

    }

    else { //parent

        printf("[%d] It's parent. Child id: %d\n", getpid(), child1_pid);
        fflush(stdout);

        char *toc11;
        asprintf(&toc11, "%d", mapDescriptor2);

        child2_pid = fork();

        if (child2_pid == -1) { //error
            printf("fork error!\n");
        }

        else if (child2_pid == 0) { //child2

            printf("[%d] It's child2\n", getpid());
            fflush(stdout);
            execl("child1.out", "child2", filename2, toc11, NULL); //execution of child2's program begins here

        }

        //parent code below

        free(filename1);
        free(filename2);
        free(toc1);
        free(toc11);

        char *str;

        while (true) {
            char c[MAX_STRING_LENTGH];
            str = fgets (c, sizeof(c), stdin);

            if(strlen(c) == 1)
                continue;

            if (str == nullptr)
                break;



            if ((strlen(c) - 1) % 2 == 0) {
                //write to the fileDescriptor2 here
            }
            else {
                //write to the fileDescriptor1 here
            }

        }

        return 0;
    }
}
