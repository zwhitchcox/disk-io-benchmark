CC=gcc

CFLAGS=-I. -std=c17 -Wall -Wextra -Werror -pedantic -pthread
CFLAGS_PROD=-O2 -D_FORTIFY_SOURCE=2
CFLAGS_DEBUG=-Og -g

.PHONY: clean
clean:
	rm -rf build

.PHONY: fio
fio:
	mkdir -p build;
	$(CC) $(CFLAGS) $(CFLAGS_PROD) $(shell ls fio/*.c) -o build/$@ -lm

.PHONY: debug
fio_debug: clean
	mkdir -p build;
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -DDEBUG $(shell ls fio/*.c) -o build/$@ -lm