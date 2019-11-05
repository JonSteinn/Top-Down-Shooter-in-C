#ifndef CupWKT1yp6_GEVENT_H
#define CupWKT1yp6_GEVENT_H

#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

/**
 * A game specific event object.
 * 
 *  quit
 *      Did the player press ESC?
 *  move_left
 *      Did the player move left this frame?
 *  move_right
 *      Did the player move right this frame?
 *  move_up
 *      Did the player move up this frame?
 *  move_down
 *      Did the player move down this frame?
 *  shoot
 *      Is the left mouse button down?
 *  mouseX
 *      The horizontal coordinate of the mouse this frame
 *  mouseY
 *      The vertical coordinate of the mouse this frame
 */
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

/**
 * Allocate resources for the event object.
 */
GameEvents* init_game_events();

/**
 * Turn SDL's events into our game events.
 */
void process_events(GameEvents* gevts);

/**
 * Free resources for the event object.
 */
void destroy_game_events(GameEvents* gevts);

#endif