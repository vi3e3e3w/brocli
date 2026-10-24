
#include <stdio.h>
#include <string.h>
#include "commands.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: brocli <command> [args...]\n");
        fprintf(stderr, "Run 'brocli help' for commands.\n");
        return 2;
    }

    if (strcmp(argv[1], "help") == 0) {
        return command_help();
    }

    if (strcmp(argv[1], "memory") == 0) {
        return command_memory();
    }

    if (strcmp(argv[1], "bluetooth") == 0) {
        return command_bluetooth();
    }

    if (strcmp(argv[1], "wifi") == 0) {
        return command_wifi(argc - 2, argv + 2);
    }

    if (strcmp(argv[1], "package") == 0) {
        return command_package(argc - 2, argv + 2);
    }

    if (strcmp(argv[1], "ping") == 0) {
        return command_ping(argc - 2, argv + 2);
    }

    fprintf(stderr, "brocli: unknown command '%s'\n", argv[1]);
    fprintf(stderr, "Run 'brocli help'.\n");
    return 2;
}

