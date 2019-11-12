CC = gcc
CFLAGS = \
	-std=c11 \
	-D_XOPEN_SOURCE=700 \
	-O2 \
	-Wall \
	-Wextra \
	-Wformat=2 \
	-DDEBUG \
	$(shell pkg-config --cflags sdl2) \
	$(shell pkg-config --cflags SDL2_image) \
	$(shell pkg-config --cflags SDL2_mixer)
OBJFLAGS = -c
LDLIBS = \
	$(shell pkg-config --libs sdl2) \
	$(shell pkg-config --libs SDL2_image) \
	$(shell pkg-config --libs SDL2_mixer)
TARGET = main

# Modules
GAME = game
CLOCK = gclock
EVENT = gevent
MATH = gmath
PLAYER = player
UTILS = utils
FLOOR = floor
SOUND = sound
COLLISION = collision
ENEMIES = enemies

DEPENDENCIES = \
	$(GAME).o \
	$(CLOCK).o \
	$(EVENT).o \
	$(MATH).o \
	$(PLAYER).o \
	$(UTILS).o \
	$(FLOOR).o \
	$(SOUND).o \
	$(COLLISION).o \
	$(ENEMIES).o

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
$(call COMPILE,FLOOR)
$(call COMPILE,SOUND)
$(call COMPILE,COLLISION)
$(call COMPILE,ENEMIES)

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGET).exe