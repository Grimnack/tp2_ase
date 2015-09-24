CC = gcc
CFLAGS= -W -Wall -pedantic -m32 -g
EXEC= main

all: $(EXEC)

main: main.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean mrproper

clean:
	rm -rf *.o ~* *.s core

mrproper: clean
	rm -rf $(EXEC)
