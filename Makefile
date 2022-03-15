CC=gcc

CFLAGS=-I.

.PHONY: debug
debug:
	$(CC) test.c -g -lm
	gdb ./a.out

.PHONY: clean
clean:
	rm -f *.test *.gch a.out

.PHONY: test
benchmark: clean
	mkdir -p build;
	$(CC) $(shell ls src/*.h) $(shell ls src/*.c) -o build/$@ -lm

.PHONY: test
benchmark.debug: clean
	$(CC) $(shell ls *.h | grep -v test) $(shell ls *.c | grep -v test) -o benchmark.debug benchmark.test.c -lm
	gdb -ex run ./benchmark.debug