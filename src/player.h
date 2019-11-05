#ifndef jPERNJXbbF_PLAYER_H
#define jPERNJXbbF_PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include "gevent.h"
#include "gmath.h"

/**
 * The player object.
 * 
 *  position
 *      The 2d coordinate of the player
 *  rotation
 *      The angle from 'east' to the direction of the player
 *  texture
 *      The player sprite
 */
typedef struct {
    Point2d*        position;
    float           rotation;
    SDL_Texture*    texture;
} Player;

/**
 * Initialize the player object by allocating memory and loading resources.
 */
Player* init_player(SDL_Renderer* renderer, float x, float y);

/**
 * Update the player based on events.
 */
void update_player(Player* player, GameEvents* gevts, float dt);

/**
 * Draw the player.
 */
void draw_player(SDL_Renderer* renderer, Player* player);

/**
 * Free any resources used by the player.
 */
void destroy_player(Player* player);

#endif