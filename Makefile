CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -pedantic -O2 -g

.PHONY: all clean

all: sched

sched: sched.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

sched.o: sched.c sched.h
	$(CC) $(CFLAGS) -c $<

queue.o: queue.c sched.h
	$(CC) $(CFLAGS) -c $<

valgrind: sched
	valgrind --leak-check=full --track-origins=yes ./sched $(ARGS)

clean:
	rm -f *.o sched

zip:
	zip -r "Randeria_CMPT360_A2.zip" A2