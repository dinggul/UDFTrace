CC = gcc

all: test1 test2

test1: test1.c
	$(CC) -o $@ $<

test2: test2.c
	$(CC) -o $@ $<

clean:
	rm -rf test1

.PHONY: all clean
