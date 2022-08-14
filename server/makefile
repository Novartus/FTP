# Makefile for the server 
# Reference: https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor
CC := gcc
CFLAGS := -Wall -g -Os
SHDIR := ../utils
OBJS = server.o $(SHDIR)/utils.o

all: server

server: $(OBJS)
	@$(CC) -o server $(CFLAGS) $(OBJS)

$(OBJS) : %.o: %.c 
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONY:
clean:
	@rm -f *.o server
	@rm -f ../utils/*.o
	@echo Done cleaning