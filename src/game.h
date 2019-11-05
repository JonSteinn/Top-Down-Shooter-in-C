#ifndef dR4wsjUCND_GAME_H
#define dR4wsjUCND_GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <SDL2/SDL.h>
#include "gclock.h"
#include "gevent.h"
#include "player.h"
#include "gmath.h"
#include "utils.h"

typedef struct {
    int32_t         width;
    int32_t         height;

    SDL_Window*     window;
    SDL_Renderer*   renderer;

    bool            running;
    GameClock*      gclock;
    GameEvents*     gevt;

    Player*         player;
} Game;


Game* init_game(int32_t argc, char** args);
void start_game(Game* game);
void destroy_game(Game* game);

#endif