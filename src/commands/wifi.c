
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"

static int run_program(char *const args[])
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        execvp(args[0], args);
        perror(args[0]);
        _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int command_wifi(int argc, char **argv)
{
    if (argc == 1 && strcmp(argv[0], "list") == 0) {
        char *args[] = {
            "nmcli", "-f", "IN-USE,SSID,SIGNAL,SECURITY",
            "device", "wifi", "list", NULL
        };
        return run_program(args);
    }

    if (argc == 2 && strcmp(argv[0], "connect") == 0) {
        char *args[] = {
            "nmcli", "device", "wifi", "connect",
            argv[1], "--ask", NULL
        };
        return run_program(args);
    }

    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  brocli wifi list\n");
    fprintf(stderr, "  brocli wifi connect <SSID>\n");
    return 2;
}

