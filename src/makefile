CC = gcc
CFLAGS = \
	-std=c11 \
	-D_XOPEN_SOURCE=700 \
	-O2 \
	-Wall \
	-Wextra \
	-Wformat=2 \
	$(shell pkg-config --cflags sdl2) \
	$(shell pkg-config --cflags SDL2_image)
OBJFLAGS = -c
LDLIBS = \
	$(shell pkg-config --libs sdl2) \
	$(shell pkg-config --libs SDL2_image)
TARGET = main

# Modules
GAME = game
CLOCK = gclock
EVENT = gevent
MATH = gmath
PLAYER = player
UTILS = utils

DEPENDENCIES = \
	$(GAME).o \
	$(CLOCK).o \
	$(EVENT).o \
	$(MATH).o \
	$(PLAYER).o \
	$(UTILS).o

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
$(call COMPILE,MATH)
$(call COMPILE,PLAYER)
$(call COMPILE,UTILS)

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGET).exe