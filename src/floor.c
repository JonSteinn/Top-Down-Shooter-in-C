#include "floor.h"

/*************
 * Bit masks *
 *************/
// Release all resources
static const uint32_t FREE_ALL = 0xFFFFFFFFu;
// Release SDL surface data
static const uint32_t FREE_SURFACE = 1u<<0;
// Free Player object
static const uint32_t FREE_MEMORY = 1u<<1;
// Release SDL texture data
static const uint32_t FREE_TEXTURE = 1u<<2;

// Path to sprite file
static const char SPRITE_PATH[] = "assets/sprites/floortile.png";
// Error message when loading image failed
static const char LOAD_IMG_LOG[] = "Floor sprite not found: %s\n";
// Error message when fails to create surface
static const char CREATE_TEXTURE_LOG[] = "Could not create texture from surface: %s\n";
// Error message when texture query fails
static const char QUERY_TEXTURE_LOG[] = "Could not query texture: %s\n";

/**
 * Function:
 *  __destroy
 *
 * Purpose:
 *  Release resources of the Floor object.
 *
 * Parameters:
 *  - floor:
 *      The Floor object.
 *  - surface:
 *      SDL_Surface that only lives in function scopes.
 *  - mask:
 *      A mask to choose which resources are destroyed.
 *      FREE_ALL
 *      FREE_SURFACE
 *      FREE_MEMORY
 *      FREE_TEXTURE
 *
 * Returns:
 *  Nothing.
 */
static void __destroy(Floor* floor, SDL_Surface* surface, uint32_t mask);

/**
 * Function:
 *  __create_texture
 *
 * Purpose:
 *  Create a texture from an existing surface.
 *
 * Parameters:
 *  - renderer:
 *      A structure that contains a rendering state.
 *  - surface:
 *      A structure that contains a collection of pixels used in software blitting.
 *  - floor
 *      The Floor object.
 *
 * Returns:
 *  true if successful, false otherwise.
 */
static bool __create_texture(SDL_Renderer* renderer, SDL_Surface* surface, Floor* floor);

/**
 * Function:
 *  __query_texture
 *
 * Purpose:
 *  Query texture for its dimension.
 *
 * Parameters:
 *  - floor:
 *      The Floor object.
 *  - surface:
 *      A structure that contains a collection of pixels used in software blitting.
 *
 * Returns:
 *  true if successful, false otherwise.
 */
static bool __query_texture(Floor* floor, SDL_Surface* surface);

/**
 * We begin by loading image and if that fails, we stop there.
 * At any point when the initialization fails, we must release
 * previously allocated resources at that point. SDL_Surface
 * does not need to be stored.
 */
Floor* init_floor(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(SPRITE_PATH);
    if (surface == NULL) {
        SDL_Log(LOAD_IMG_LOG, SDL_GetError());
        return NULL;
    }

    Floor* floor = (Floor*)malloc(sizeof(Floor));

    if (!__create_texture(renderer, surface, floor)) return NULL;
    if (!__query_texture(floor, surface)) return NULL;

    SDL_FreeSurface(surface);

    return floor;
}

/**
 * We travel left to right, then top to bottom and draw one tile at a time.
 */
void draw_floor(SDL_Renderer* renderer, Floor* floor, int32_t w, int32_t h) {
    for (int32_t y = 0; y <= h; y += floor->texture_height) {
        for (int32_t x = 0; x <= w; x += floor->texture_width) {
            SDL_Rect rect = { x, y, floor->texture_width, floor->texture_height } ;
            SDL_RenderCopy(renderer, floor->texture, NULL, &rect);
        }
    }
}

/**
 * Does not need to release the surface, hence we
 * do not use the FREE_ALL mask.
 */
void destroy_floor(Floor* floor) {
    __destroy(floor, NULL, FREE_TEXTURE | FREE_MEMORY);
}

/**
 * Check each resources against mask before releasing.
 */
static void __destroy(Floor* floor, SDL_Surface* surface, uint32_t mask) {
    if (mask & FREE_SURFACE) SDL_FreeSurface(surface);
    if (mask & FREE_TEXTURE) SDL_DestroyTexture(floor->texture);
    if (mask & FREE_MEMORY) free(floor);
}

/**
 * Releases the surface resources and Floor memory if we fail
 * to create the texture.
 */
static bool __create_texture(SDL_Renderer* renderer, SDL_Surface* surface, Floor* floor) {
    floor->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (floor->texture == NULL) {
        SDL_Log(CREATE_TEXTURE_LOG, SDL_GetError());
        __destroy(floor, surface, FREE_SURFACE | FREE_MEMORY);
        return false;
    }
    return true;
}

/**
 * Releases the surface and texture resources and Floor memory
 * if we fail to query the texture.
 */
static bool __query_texture(Floor* floor, SDL_Surface* surface) {
    if (SDL_QueryTexture(floor->texture, NULL, NULL, &floor->texture_width, &floor->texture_height) < 0) {
        SDL_Log(QUERY_TEXTURE_LOG, SDL_GetError());
        __destroy(floor, surface, FREE_ALL);
        return false;
    }
    return true;
}