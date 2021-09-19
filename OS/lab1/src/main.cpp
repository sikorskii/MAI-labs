#include "unistd.h"
#include <cstdio>
#include <cstdlib>

int main() {
	int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe error!");
    }

    char buf[20];
    char *filename1;
    char *filename2;

    printf("Enter file1 name\n");
    scanf("%s", buf);

    asprintf(&filename1, "%s", buf);

    printf("Enter file2 name\n");
    scanf("%s", buf);

    asprintf(&filename2, "%s", buf);

    pid_t child1_pid, child2_pid;

    pipe(pipe1);
    pipe(pipe2);

    char *toc1;
    char *toc2;
    asprintf(&toc1, "%d", pipe1[0]);
    asprintf(&toc2, "%d", pipe1[1]);

    char *toc11;
    char *toc22;
    asprintf(&toc11, "%d", pipe2[0]);
    asprintf(&toc22, "%d", pipe2[1]);

    child1_pid = fork();

    if (child1_pid == -1) {
        printf("fork error!\n");
    }

    else if (child1_pid == 0) { //child1
        printf("[%d] It's child1\n", getpid());
        fflush(stdout);
        execl("child1.out", filename1, toc1, toc2, NULL); //execution of child1's program begins here
    }

    else { //parent
        printf("[%d] It's parent. Child id: %d\n", getpid(), child1_pid);
        fflush(stdout);
        child2_pid = fork();

        if (child2_pid == -1) { //error
            printf("fork error!\n");
        }

        else if (child2_pid == 0) { //child2
            printf("[%d] It's child2\n", getpid());
            fflush(stdout);
            execl("child2.out", filename2, toc11, toc22, NULL); //execution of child2's program begins here
        }

        //parent code below

        free(filename1);
        free(filename2);
        //printf("%d", fd);
        return 0;
    }
}
