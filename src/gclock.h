#ifndef JqdBnUmofN_GCLOCK_H
#define JqdBnUmofN_GCLOCK_H

#include <stdlib.h>
#include <SDL2/SDL.h>

/**
 * A game clock object that keeps hold of our framerate.
 * 
 *  now
 *      The time this frame
 *  last
 *      The time last frame
 *  dt
 *      The time between the frames
 *  fps
 *      1 / dt
 */
typedef struct {
    Uint64  now;
    Uint64  last;
    float   dt;
    float   fps;
} GameClock;

/**
 * Allocate resources for the game clock.
 */
GameClock* init_game_clock();

/**
 * Calculate the time between frames.
 */
void update_game_clock(GameClock* gclock);

/**
 * Free resources for the game clock.
 */
void destroy_game_clock(GameClock* gclock);

#endif