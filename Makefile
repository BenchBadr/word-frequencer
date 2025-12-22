CC = clang

CFLAGS = -Wall
SRCS = main.c util/gererMem.c algos/adt.c util/lecture.c

OBJS = $(SRCS:.c=.o)

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main

clean:
	rm -f **/*.o *.o main
