CC = gcc
CPPFLAGS =
CFLAGS = -std=c11 -D_XOPEN_SOURCE=700 -O2 -Wall -Wextra -Wformat=2 $(shell pkg-config --cflags sdl2)
LDFLAGS =
LOADLIBES =
LDLIBS = $(shell pkg-config --libs sdl2)

.DEFAULT: all
.PHONY: all
all: main

clean:
	rm -f *.o

distclean: clean
	rm -f main