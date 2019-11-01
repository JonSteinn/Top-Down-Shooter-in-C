#ifndef JqdBnUmofN_GCLOCK_H
#define JqdBnUmofN_GCLOCK_H

#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct {
    Uint64  now;
    Uint64  last;
    float   dt;
    float   fps;
} GameClock;

GameClock* init_game_clock();
void update_game_clock(GameClock* gclock);
void destroy_game_clock(GameClock* gclock);

#endif