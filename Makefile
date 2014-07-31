CC = gcc
CFLAGS = -Wall -g
IGNORE = -Wno-pointer-sign 
SOURCES = command.c reminder.c main.c list.c
LIBS = -lsqlite3
OBJS = $(SOURCES: .c=.o)
test: $(OBJS)

	$(CC) $(CFLAGS) -o test $(OBJS) $(LIBS) $(IGNORE)
