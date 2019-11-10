#include "gclock.h"

/**
 * Allocate memory for the GameClock and set all fields to 0
 * except for GameClock::now, which is set to the current value
 * of the high resolution counter.
 */
GameClock* init_game_clock(void) {
    GameClock* c = (GameClock*)malloc(sizeof(GameClock));

    c->now = SDL_GetPerformanceCounter();
    c->last = 0;
    c->dt = 0.0f;
    c->fps = 0.0f;

    return c;
}

/**
 * Calculate the time since this function was last called
 * and set gclock->dt to that value, while gclock->fps is
 * its multiplicative inverse.
 */
void update_game_clock(GameClock* gclock) {
    gclock->last = gclock->now;
    gclock->now = SDL_GetPerformanceCounter();
    gclock->dt = (float)((gclock->now - gclock->last)*1000 / (float)SDL_GetPerformanceFrequency() );
    gclock->fps = 1.0f/gclock->dt;
}

/**
 * Free memory of GameClock struct.
 */
void destroy_game_clock(GameClock* gclock) {
    free(gclock);
}