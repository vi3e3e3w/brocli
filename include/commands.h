#ifndef BROCLI_COMMANDS_H
#define BROCLI_COMMANDS_H

int command_help(void);
int command_memory(void);
int command_bluetooth(void);
int command_wifi(int argc, char **argv);
int command_package(int argc, char **argv);
int command_ping(int argc, char **argv);

#endif

