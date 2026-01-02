CC = clang

CFLAGS = -Wall

SRCS = main.c util/gererMem.c algos/adt.c util/lecture.c algos/aldi.c

OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main

clean:
	rm -f $(OBJS) main
