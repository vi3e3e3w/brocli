#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT_SIZE 1024

static volatile sig_atomic_t interrupted = 0;

static void on_sigint(int sig) {
    (void)sig;
    interrupted = 1;
}

static int run_program(char *const argv[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        execvp(argv[0], argv);
        perror(argv[0]);
        _exit(127);
    }

    int status;
    while (waitpid(pid, &status, 0) < 0) {
        if (interrupted) break;
        perror("waitpid");
        return -1;
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

static void trim(char *s) {
    size_t len = strlen(s);

    while (len && isspace((unsigned char)s[len - 1]))
        s[--len] = '\0';

    size_t start = 0;
    while (s[start] && isspace((unsigned char)s[start]))
        start++;

    if (start)
        memmove(s, s + start, strlen(s + start) + 1);
}

static int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);

    if (!fgets(buf, (int)size, stdin))
        return 0;

    if (!strchr(buf, '\n') && !feof(stdin)) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        puts("Input too long.");
        buf[0] = '\0';
        return 1;
    }

    trim(buf);
    return 1;
}

static void show_help(void) {
    puts("\n======= AVAILABLE COMMANDS =======");
    puts("help           Show this help");
    puts("check package  Run sudo apt update");
    puts("memory usage   Show RAM usage");
    puts("bluetooth      Open bluetoothctl");
    puts("wifi           Enter Wi-Fi submenu");
    puts("ping network   Ping a host");
    puts("clear          Clear terminal");
    puts("exit           Exit BroCommand");
}

static void wifi_menu(void) {
    char input[INPUT_SIZE];

    while (!interrupted) {
        if (!read_line("\033[1;36mtool/wifi>\033[0m ", input, sizeof input))
            return;

        if (interrupted) break;

        if (!strcmp(input, "exit"))
            break;

        if (!strcmp(input, "help")) {
            puts("Commands: list, connect <SSID>, help, exit");
        } else if (!strcmp(input, "list")) {
            char *args[] = {
                "nmcli", "-f", "IN-USE,SSID,SIGNAL,SECURITY",
                "device", "wifi", "list", NULL
            };
            run_program(args);
        } else if (!strncmp(input, "connect ", 8) &&
                   input[8] != '\0') {
            char *ssid = input + 8;

            char *args[] = {
                "nmcli", "device", "wifi", "connect", ssid,
                "--ask", NULL
            };

            puts("Connecting...");
            if (run_program(args) == 0)
                puts("Connected!");
            else
                puts("Connection failed.");
        } else {
            printf("Unknown command: %s\n", input);
        }
    }

    interrupted = 0;
}

static void ping_menu(void) {
    char host[INPUT_SIZE];

    if (!read_line("Host or IP to ping> ", host, sizeof host))
        return;

    if (!host[0] || host[0] == '-') {
        puts("Invalid host.");
        return;
    }

    char *args[] = {"ping", "-c", "10", host, NULL};
    run_program(args);
}

static void package_update(void) {
    char confirm[32];

    puts("This will run: sudo apt update");
    puts("Administrator authentication may be required.");

    if (!read_line("Continue? [y/N] ", confirm, sizeof confirm))
        return;

    if (!strcmp(confirm, "y") || !strcmp(confirm, "Y")) {
        char *args[] = {"sudo", "apt", "update", NULL};
        run_program(args);
    } else {
        puts("Cancelled.");
    }
}

int main(void) {
    struct sigaction sa = {0};
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    puts("\033[1;36mBroCommand\033[0m");
    puts("Type help for commands.");

    char input[INPUT_SIZE];

    while (1) {
        interrupted = 0;

        if (!read_line("\033[1;36mtool> \033[0m",
                       input, sizeof input))
            break;

        if (interrupted) {
            puts("\nType exit to quit.");
            continue;
        }

        if (!input[0])
            continue;

        if (!strcmp(input, "help")) {
            show_help();
        } else if (!strcmp(input, "check package")) {
            package_update();
        } else if (!strcmp(input, "memory usage")) {
            char *args[] = {"free", "-h", NULL};
            run_program(args);
        } else if (!strcmp(input, "bluetooth")) {
            char *args[] = {"bluetoothctl", NULL};
            run_program(args);
        } else if (!strcmp(input, "wifi")) {
            wifi_menu();
        } else if (!strcmp(input, "ping network")) {
            ping_menu();
        } else if (!strcmp(input, "clear")) {
            printf("\033[3J\033[H\033[2J");
            fflush(stdout);
        } else if (!strcmp(input, "exit")) {
            puts("bye");
            break;
        } else {
            printf("%s: command not found; type help\n", input);
        }
    }

    return 0;
}

