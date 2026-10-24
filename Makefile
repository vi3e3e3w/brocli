CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude
TARGET = brocli

SOURCES = \
	src/main.c \
	src/commands/help.c \
	src/commands/memory.c \
	src/commands/bluetooth.c \
	src/commands/wifi.c \
	src/commands/package.c \
	src/commands/ping.c

OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

src/%.o: src/%.c include/commands.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

