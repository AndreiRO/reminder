CC = gcc
CFLAGS = -Wall -g
IGNORE = -Wno-pointer-sign 
SOURCES = src/reminder.c src/main.c src/list.c src/command.c src/str.c
LIBS = -lsqlite3
OBJS = $(SOURCES: .c=.o)
test: $(OBJS)

	$(CC) $(CFLAGS) -o test $(OBJS) $(LIBS) $(IGNORE)
