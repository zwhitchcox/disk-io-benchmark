CC=gcc

CFLAGS=-I.

.PHONY: clean
clean:
	rm -rf build

.PHONY: fio
fio:
	mkdir -p build;
	$(CC) $(shell ls fio/*.h) $(shell ls fio/*.c) -o build/$@ -lm

fio_verify:
	mkdir -p build;
	$(CC) -DVERIFY $(shell ls fio/*.h) $(shell ls fio/*.c) -o build/$@ -lm

.PHONY: debug
fio_debug: clean
	mkdir -p build;
	$(CC) -DDEBUG $(shell ls fio/*.h) $(shell ls fio/*.c) -o build/$@ -lm