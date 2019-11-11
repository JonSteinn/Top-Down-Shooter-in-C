#ifndef jPERNJXbbF_PLAYER_H
#define jPERNJXbbF_PLAYER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "gevent.h"
#include "gmath.h"
#include "collision.h"

/**
 * Struct:
 *  Player
 *
 * Purpose:
 *  Holds on to all player related resources.
 *
 * Fields
 *  - texture:
 *      A structure that contains an efficient, driver-specific
 *      representation of pixel data for the player.
 *  - texture_width:
 *      The width of the player texture in pixels.
 *  - texture_height:
 *      The height of the player texture in pixels.
 *  - position:
 *      The 2d position of the player.
 *  - rotation:
 *      The direction the player is facing in radians.
 *  - collider:
 *      The geometric object to calculate collision for.
 */
typedef struct {
    SDL_Texture*    texture;
    int32_t         texture_width;
    int32_t         texture_height;
    Point2d         position;
    float           rotation;
    Collider        collider;
} Player;

/**
 * Function:
 *  init_player
 *
 * Purpose:
 *  Create and initialize a Player object.
 *
 * Parameters:
 *  - renderer:
 *      A structure that contains a rendering state.
 *  - x:
 *      The horizontal starting position of the player.
 *  - y:
 *      The vertical starting position of the player.
 *
 * Returns:
 *  Player object if successful, NULL otherwise.
 */
Player* init_player(SDL_Renderer* renderer, float x, float y);

/**
 * Function:
 *  update_player
 *
 * Purpose:
 *  Update the player based on events.
 *
 * Parameters:
 *  - player:
 *      The player object.
 *  - gevts:
 *      The events that occured this frame.
 *  - dt:
 *      Delta time.
 *  - w:
 *      The window's width.
 *  - h:
 *      The window's height.
 *
 * Returns:
 *  Nothing.
 */
void update_player(Player* player, GameEvents* gevts, float dt, int32_t w, int32_t h);

/**
 * Function:
 *  draw_player
 *
 * Purpose:
 *  Draw the player. This should always be called after update_player.
 *
 * Parameters:
 *  - renderer:
 *      A structure that contains a rendering state.
 *  - player:
 *      The player object.
 *
 * Returns:
 *  Nothing.
 */
void draw_player(SDL_Renderer* renderer, Player* player);

/**
 * Free any resources used by the player.
 */
/**
 * Function:
 *  destroy_player
 *
 * Purpose:
 *  Release all resources of the player object.
 *
 * Parameters:
 *  - player:
 *      The player object to destroy.
 *
 * Returns:
 *  Nothing.
 */
void destroy_player(Player* player);

#endif