#ifndef hHj7xpRn1j_FLOOR_H
#define hHj7xpRn1j_FLOOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * Struct:
 *  Floor
 * 
 * Purpose:
 *  Holds on to all floor related resources.
 * 
 * Fields:
 *  - texture:
 *      A structure that contains an efficient, driver-specific 
 *      representation of pixel data for the floor.
 *  - texture_width:
 *      The width of the player texture in pixels.
 *  - texture_height:
 *      The height of the player texture in pixels.
 */
typedef struct {
    SDL_Texture*    texture;
    int32_t         texture_width;
    int32_t         texture_height;
} Floor;

/**
 * Function:
 *  init_floor
 * 
 * Purpose:
 *  Create and initialize a Floor object.
 * 
 * Parameters:
 *  - renderer:
 *      A structure that contains a rendering state.
 * 
 * Returns:
 *  The Floor object.
 */
Floor* init_floor(SDL_Renderer* renderer);

/**
 * Function:
 *  draw_floor
 * 
 * Purpose:
 *  Fills the entire window with floor tiles.
 * 
 * Parameters:
 *  - renderer:
 *      A structure that contains a rendering state.
 *  - floor:
 *      The floor object to draw.
 *  - w:
 *      The window's width.
 *  - h:
 *      The window's height.
 * 
 * Returns:
 *  Nothing.
 */
void draw_floor(SDL_Renderer* renderer, Floor* floor, int32_t w, int32_t h);

/**
 * Function:
 *  destroy_floor
 * 
 * Purpose:
 *  Release any resources held by the Floor object.
 * 
 * Parameters:
 *  - floor:
 *      The floor object to destroy.
 * 
 * Returns:
 *  Nothing.
 */
void destroy_floor(Floor* floor);


#endif