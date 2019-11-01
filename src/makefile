CC = gcc
CFLAGS = -std=c11 -D_XOPEN_SOURCE=700 -O2 -Wall -Wextra -Wformat=2 $(shell pkg-config --cflags sdl2)
OBJFLAGS = -c
LDLIBS = $(shell pkg-config --libs sdl2)
TARGET = main

# Modules
GAME = game
CLOCK = gclock
EVENT = gevent

DEPENDENCIES = \
	$(GAME).o \
	$(CLOCK).o \
	$(EVENT).o

define COMPILE
$(1).o: $(1).c
	$(CC) $($(1)).c $(CFLAGS) $(LDLIBS) $(OBJFLAGS)
endef

.DEFAULT: all
.PHONY: all
all: $(TARGET)

$(TARGET): $(TARGET).o $(DEPENDENCIES)
	$(CC) $(TARGET).o $(DEPENDENCIES) -o $(TARGET).exe $(LDLIBS)

$(call COMPILE,TARGET)
$(call COMPILE,GAME)
$(call COMPILE,CLOCK)
$(call COMPILE,EVENT)

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGET).exe