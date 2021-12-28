#include <iostream>

#include "ServerNode.h"
#include <sys/wait.h>
#include "SpringBootApplication.h"

void child(int sig) {
    pid_t pid;
    pid = wait(nullptr);
}

int main() {
    signal(SIGCHLD, child);
    SpringBootApplication app;
    app.run();
}
