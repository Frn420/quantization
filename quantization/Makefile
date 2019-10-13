CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

SHELL = /bin/sh

.PHONY: all clean

all: quantization

ignore-comments.o: ignore-comments.c ignore-comments.h order-operations.h
	$(CC) $(CFLAGS) -c $<

order-operations.o: order-operations.c order-operations.h
	$(CC) $(CFLAGS) -c $<

orders.o: orders.c orders.h order-operations.h trie.h
	$(CC) $(CFLAGS) -c $<

trie.o: trie.c trie.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c ignore-comments.h order-operations.h orders.h trie.h
	$(CC) $(CFLAGS) -c $<

OBJECTS = main.o ignore-comments.o order-operations.o orders.o trie.o

quantization: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm quantization $(OBJECTS)
