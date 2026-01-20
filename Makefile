CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: allocator_demo

test: allocator_demo
	./allocator_demo

allocator_demo: main.o allocator.o
	$(CC) $(CFLAGS) -o allocator_demo main.o allocator.o

main.o: main.c allocator.h
	$(CC) $(CFLAGS) -c main.c

allocator.o: allocator.c allocator.h
	$(CC) $(CFLAGS) -c allocator.c

clean:
	rm -f *.o allocator_demo
