ifeq ($(DEBUG), 1)
	MODE = -g -O0 -DDEBUG
else
	MODE = -O2 -DNDEBUG
endif

CC = gcc
CFLAGS = \
	-std=c11 \
	$(MODE) \
	-Wall \
	-Wextra \
	-Wformat=2 \
	-Wfatal-errors \
	-Werror \
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
LIST = list
BULLETS = bullets

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
	$(ENEMIES).o \
	$(LIST).o \
	$(BULLETS).o

define COMPILE
$(1).o: $(1).c
	$(CC) $($(1)).c $(CFLAGS) $(LDLIBS) $(OBJFLAGS)
endef

.DEFAULT: all
.PHONY: all
all: $(TARGET)

$(TARGET): $(TARGET).o $(DEPENDENCIES)
	$(CC) $(TARGET).o $(DEPENDENCIES) $(CFLAGS) -o $(TARGET).exe $(LDLIBS)

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
$(call COMPILE,LIST)
$(call COMPILE,BULLETS)

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGET).exe