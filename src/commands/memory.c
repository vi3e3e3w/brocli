#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include "commands.h"

int command_memory(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        execlp("free", "free", "-h", (char *)NULL);
        perror("free");
        _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

