SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
CC:=gcc
client:$(OBJS)
	$(CC) $^ -o $@ -lpthread -lmysqlclient
%.o:%.c
	$(CC) -c $^ -o $@ -g
clean:
	$(RM) $(OBJS)
include:
	sudo vim /usr/include/kk.h
