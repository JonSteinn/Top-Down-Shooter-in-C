#ifndef CupWKT1yp6_GEVENT_H
#define CupWKT1yp6_GEVENT_H

#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
    bool quit;
    bool move_left;
    bool move_right;
    bool move_up;
    bool move_down;
    bool shoot;
    int mouseX;
    int mouseY;
} GameEvents;

GameEvents* init_game_events();
void process_events(GameEvents* gevts);
void destroy_game_events(GameEvents* gevts);

#endif