#ifndef CupWKT1yp6_GEVENT_H
#define CupWKT1yp6_GEVENT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

/**
 * Struct:
 *  GameEvents
 * 
 * Purpose:
 *  A game specific mapping of input events.
 * 
 * Fields
 *  - quit:
 *      Did the player press ESC or close window?
 *  - move_left:
 *      Did the player move left this frame?
 *  - move_right:
 *      Did the player move right this frame?
 *  - move_up:
 *      Did the player move up this frame?
 *  - move_down:
 *      Did the player move down this frame?
 *  - shoot:
 *      Is the left mouse button down?
 *  - mouseX:
 *      The horizontal coordinate of the mouse this frame.
 *  - mouseY:
 *      The vertical coordinate of the mouse this frame.
 */
typedef struct {
    bool        quit:1;
    bool        move_left:1;
    bool        move_right:1;
    bool        move_up:1;
    bool        move_down:1;
    bool        shoot:1;
    int32_t     mouseX;
    int32_t     mouseY;
} GameEvents;

/**
 * Function:
 *  init_game_events
 * 
 * Purpose:
 *  Create and intialize GameEvents object.
 * 
 * Parameters:
 *  None.
 * 
 * Returns:
 *  A GameEvents object.
 */
GameEvents* init_game_events();

/**
 * Function:
 *  process_events
 * 
 * Purpose:
 *  Check which events happened during this frame.
 * 
 * Parameters:
 *  - gevts:
 *      The GameEvents object to process.
 * 
 * Returns:
 *  Nothing.
 */
void process_events(GameEvents* gevts);

/**
 * Function:
 *  destroy_game_events
 * 
 * Purpose:
 *  Free resources for the GameEvents object.
 * 
 * Parameters:
 *  - gevts:
 *      The GameEvents object to destroy.
 * 
 * Returns:
 *  Nothing.
 */
void destroy_game_events(GameEvents* gevts);

#endif