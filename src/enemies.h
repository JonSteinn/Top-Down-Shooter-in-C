#ifndef M3cvCxcSjG_ENEMIES_H
#define M3cvCxcSjG_ENEMIES_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "gmath.h"

/**
 * Struct:
 *  Enemy
 *
 * Purpose:
 *  Holds on to all resources needed for a single enemy.
 *
 * Fields:
 *  - position:
 *      The 2d position of the enemy.
 *  - rotation:
 *      The direction the enemy is facing.
 *  - state:
 *      Which texture to render.
 */
typedef struct {
    Point2d     position;
    float       rotation;
    float       state;
} Enemy;

/**
 * Struct:
 *  Enemies
 *
 * Purpose:
 *  Holds on to the collection of all enemies and
 *  any resources shared between all of them.
 *
 * Fields:
 *  - texture:
 *      A structure that contains an efficient, driver-specific
 *      representation of pixel data for the enemy spritesheet.
 *  - texture_states:
 *      The positions of the enemy textures within the
 *      spritesheet in order.
 *  - enemies:
 *      An array of enemies.
 *  - max_enemies:
 *      The element count of the enemies array.
 *  - collision_radius:
 *      The width (or height) of the enemy, divided by 2.
 */
typedef struct {
    SDL_Texture*    texture;
    SDL_Rect        texture_states[6];
    Enemy*          enemies;
    int32_t         max_enemies;
    float           collision_radius;
} Enemies;

/**
 * Function:
 *  init_enemies
 *
 * Purpose:
 *
 * Parameters:
 *  - renderer:
 *      A structure that contains a rendering state.
 *  - max_enemies:
 *  - w:
 *      The window's width.
 *  - h:
 *      The window's height.
 *
 * Returns:
 *  Enemies object if successful, NULL otherwise.
 */
Enemies* init_enemies(SDL_Renderer* renderer, int32_t max_enemies, int32_t w, int32_t h);

/**
 * Function:
 *  update_enemies
 *
 * Purpose:
 *  Update the enemies within the game.
 *
 * Parameters:
 *  - enemies:
 *      The Enemies object to update.
 *  - dt:
 *      Delta time.
 *  - p_pos:
 *      The position of the player.
 *
 * Returns:
 *  Nothing.
 */
void update_enemies(Enemies* enemies, float dt, Point2d* p_pos);

/**
 * Function:
 *  draw_enemies
 *
 * Purpose:
 *  Draw the enemies. This should always be called after update_enemies.
 *
 * Parameters:
 *  - renderer:
 *      A structure that contains a rendering state.
 *  - enemies:
 *      The Enemies object to draw.
 *  - w:
 *      The window's width.
 *  - h:
 *      The window's height.
 *
 * Returns:
 *  Nothing.
 */
void draw_enemies(SDL_Renderer* renderer, Enemies* enemies, int32_t w, int32_t h);

/**
 * Function:
 *  destroy_enemies
 *
 * Purpose:
 *  Free any sources related to enemies.
 *
 * Parameters:
 *  - enemies:
 *      The Enemies object to destroy.
 *
 * Returns:
 *  Nothing.
 */
void destroy_enemies(Enemies* enemies);

#endif