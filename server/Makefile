SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
CC:=gcc
server:$(OBJS)
	$(CC) $^ -o $@ -lpthread -lmysqlclient -lcrypt
%.o:%.c
	$(CC) -c $^ -o $@ -g
clean:
	$(RM) $(OBJS)
include:
	sudo vim /usr/include/kk.h
