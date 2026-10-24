
#include <stdio.h>
#include "commands.h"

int command_ping(int argc, char **argv)
{
    (void)argv;

    if (argc > 2) {
        fprintf(stderr, "Usage: brocli ping [host [count]]\n");
        return 2;
    }

    puts("BroCLI ICMP ping");
    puts("ICMP engine is not implemented yet.");
    puts("Next: DNS resolution, ICMP Echo, timeout and statistics.");

    return 0;
}

