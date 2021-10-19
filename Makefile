CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
	$(CC) $(CFLAGS) -o 9cc $(OBJS) $(LDFLAGS)

$(OBJS): 9cc.h

build:
	docker build ./ -t compilerbook

test: 9cc
	./test.sh

down:
	docker rmi compilerbook

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean
