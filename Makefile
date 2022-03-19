CC=gcc

CFLAGS=-I.

.PHONY: clean
clean:
	rm -f *.test *.gch a.out

.PHONY: test
benchmark: clean
	mkdir -p build;
	$(CC) $(shell ls src/*.h) $(shell ls src/*.c) -o build/$@ -lm

.PHONY: test
debug: clean
	$(CC) -ggdb $(shell ls src/*.h) $(shell ls src/*.c) -o build/benchmark -lm
	gdb -ex run --args ./build/benchmark ./testing.txt