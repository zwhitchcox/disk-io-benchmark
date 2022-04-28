CC=gcc

CFLAGS=-I. -std=c17 -Wall -Wextra -Werror -pedantic -pthread
CFLAGS_PROD=-O2 -D_FORTIFY_SOURCE=2
CFLAGS_DEBUG=-Og -g

.PHONY: clean
clean:
	rm -rf build

.PHONY: benchmark
benchmark:
	mkdir -p build;
	$(CC) $(CFLAGS) $(CFLAGS_PROD) $(shell ls benchmark/*.c) -o build/$@ -lm

.PHONY: debug
benchmark_debug: clean
	mkdir -p build;
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -DDEBUG $(shell ls benchmark/*.c) -o build/$@ -lm