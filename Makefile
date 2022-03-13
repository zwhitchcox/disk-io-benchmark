CC=gcc

CFLAGS=-I.

.PHONY: debug
debug:
	$(CC) test.c -g -lm
	gdb ./a.out

.PHONY: clean
clean:
	rm -f *.test

.PHONY: test
benchmark: clean
	$(CC) $(shell ls *.h | grep -v benchmark) $(shell ls *.c | grep -v benchmark) -o $@ $@.c -lm
	sudo ./$@ src dest

.PHONY: test
benchmark.debug: clean
	$(CC) $(shell ls *.h | grep -v test) $(shell ls *.c | grep -v test) -o benchmark.debug benchmark.test.c -lm
	gdb -ex run ./benchmark.debug