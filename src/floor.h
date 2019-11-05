#ifndef hHj7xpRn1j_FLOOR_H
#define hHj7xpRn1j_FLOOR_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    SDL_Texture*    texture;
    int32_t         texture_width;
    int32_t         texture_height;
} Floor;
    
Floor* init_floor(SDL_Renderer* renderer);
void draw_floor(SDL_Renderer* renderer, Floor* floor, int32_t w, int32_t h);
void destroy_floor(Floor* floor);


#endif