#ifndef JqdBnUmofN_GCLOCK_H
#define JqdBnUmofN_GCLOCK_H

#include <stdlib.h>

#include <SDL2/SDL.h>

/**
 * Struct:
 *  GameClock
 * 
 * Purpose:
 *  Keeps track of time between frames.
 * 
 * Fields:
 *  - now: 
 *      The time this frame.
 *  - last: 
 *      The time last frame.
 *  - dt: 
 *      The time between frames in seconds.
 *  - fps: 
 *      The multiplicative inverse of dt.
 */
typedef struct {
    Uint64  now;
    Uint64  last;
    float   dt;
    float   fps;
} GameClock;

/**
 * Function:
 *  init_game_clock
 * 
 * Purpose:
 *  Create and intialize GameClock object.
 * 
 * Parameters:
 *  None.
 * 
 * Returns:
 *  A GameClock object.
 */
GameClock* init_game_clock();

/**
 * Function:
 *  update_game_clock
 * 
 * Purpose:
 *  Calculate the time between frames.
 * 
 * Parameters:
 *  The GameClock object.
 * 
 * Returns:
 *  Nothing.
 */
void update_game_clock(GameClock* gclock);

/**
 * Function:
 *  destroy_game_clock
 * 
 * Purpose:
 *  Free resources for the GameClock object.
 * 
 * Parameters:
 *  The GameClock object.
 * 
 * Returns:
 *  Nothing.
 */
void destroy_game_clock(GameClock* gclock);

#endif