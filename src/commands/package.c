
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"

int command_package(int argc, char **argv)
{
    if (argc != 1 || strcmp(argv[0], "update") != 0) {
        fprintf(stderr, "Usage: brocli package update\n");
        return 2;
    }

    char answer[16];

    printf("This will run: sudo apt update\n");
    printf("Continue? [y/N] ");
    fflush(stdout);

    if (fgets(answer, sizeof(answer), stdin) == NULL) {
        return 1;
    }

    if (answer[0] != 'y' && answer[0] != 'Y') {
        puts("Cancelled.");
        return 0;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        execlp("sudo", "sudo", "apt", "update", (char *)NULL);
        perror("sudo");
        _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

