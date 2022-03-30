CC=gcc

CFLAGS=-I.

.PHONY: clean
clean:
	rm -f *.test *.gch a.out

.PHONY: test
fio: clean
	mkdir -p build;
	$(CC) $(shell ls fio/*.h) $(shell ls fio/*.c) -o build/$@ -lm

.PHONY: test
debug: clean
	$(CC) -ggdb $(shell ls fio/*.h) $(shell ls fio/*.c) -o build/benchmark -lm
	gdb -ex run --args ./build/benchmark ./testing.txt