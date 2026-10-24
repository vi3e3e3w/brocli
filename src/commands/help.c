#include <stdio.h>
#include "commands.h"

int command_help(void)
{
    puts("\033[1mBroCLI - Command Line Tools\033[0m");
    puts("");
    puts("\033[1mUsage:\033[0m brocli <command> [args...]");
    puts("");
    puts("\033[1mCommands:\033[0m");
    puts("  \033[1mhelp\033[0m                  Show this help");
    puts("  \033[1mmemory\033[0m                Show RAM usage");
    puts("  \033[1mbluetooth\033[0m             Open bluetoothctl");
    puts("  \033[1mwifi list\033[0m             List Wi-Fi networks");
    puts("  \033[1mwifi connect <SSID>\033[0m   Connect to a Wi-Fi network");
    puts("  \033[1mpackage update\033[0m        Update APT package lists");
    puts("  \033[1mping\033[0m                  Start interactive ICMP ping");
    puts("  \033[1mping <host> [count]\033[0m   Ping a host");
    return 0;
}
