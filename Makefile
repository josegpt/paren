CFLAGS=-Wall -Werror -pedantic --std=c11

all: calc

calc: main.o lexer.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm calc *.o

.PHONY: all clean
