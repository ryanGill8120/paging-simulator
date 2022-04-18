CC = gcc
COPS = -D_GNU_SOURCE -g

all: part1 part2

part1: part1.c
	$(CC) $(COPS) -o part1 part1.c

part2: part2.c pagetable.c phypages.c pagetable.h phypages.h
	$(CC) $(COPS) -o part2 part2.c pagetable.c phypages.c

clean:
	rm part1-output part2-output