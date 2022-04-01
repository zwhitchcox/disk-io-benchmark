CC=gcc

CFLAGS=-I.

.PHONY: clean
clean:
	rm -f *.test *.gch a.out

.PHONY: fio
fio:
	mkdir -p build;
	$(CC) $(shell ls fio/*.h) $(shell ls fio/*.c) -o build/$@ -lm

fio_verify:
	mkdir -p build;
	$(CC) -DVERIFY $(shell ls fio/*.h) $(shell ls fio/*.c) -o build/$@ -lm

.PHONY: debug
debug: clean
	$(CC) -ggdb $(shell ls fio/*.h) $(shell ls fio/*.c) -o build/benchmark -lm
	gdb -ex run --args ./build/benchmark ./testing.txt