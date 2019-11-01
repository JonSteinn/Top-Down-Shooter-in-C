#include "gclock.h"


GameClock* init_game_clock() {
    GameClock* c = (GameClock*)malloc(sizeof(GameClock));

    c->now = SDL_GetPerformanceCounter();
    c->last = 0;
    c->dt = 0.0f;
    c->fps = 0.0f;

    return c;
}

void update_game_clock(GameClock* gclock) {
    gclock->last = gclock->now;
    gclock->now = SDL_GetPerformanceCounter();
    gclock->dt = (float)((gclock->now - gclock->last)*1000 / (float)SDL_GetPerformanceFrequency() );
    gclock->fps = 1.0f/gclock->dt;
}

void destroy_game_clock(GameClock* gclock) {
    free(gclock);
}