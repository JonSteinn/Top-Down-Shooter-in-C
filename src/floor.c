#include "floor.h"


    
Floor* init_floor(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("assets/sprites/floortile.png");
    if (surface == NULL) {
        SDL_Log("Floor sprite not found: %s\n", SDL_GetError());
        return NULL;
    }

    Floor* floor = (Floor*)malloc(sizeof(Floor));

    floor->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (floor->texture == NULL) {
        SDL_Log("Could not create texture from surface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(floor);
        return NULL;   
    }

    if (SDL_QueryTexture(floor->texture, NULL, NULL, &floor->texture_width, &floor->texture_height) < 0) {
        SDL_Log("Could not query texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(floor->texture);
        free(floor);
        return NULL;   
    }
    
    SDL_FreeSurface(surface);

    return floor;
}

void draw_floor(SDL_Renderer* renderer, Floor* floor, int32_t w, int32_t h) {
    for (int32_t y = 0; y <= h; y += floor->texture_height) {
        for (int32_t x = 0; x <= w; x += floor->texture_width) {
            SDL_Rect rect = { x, y, floor->texture_width, floor->texture_height } ;
            SDL_RenderCopy(renderer, floor->texture, NULL, &rect);
        }
    }    
}

void destroy_floor(Floor* floor) {
    SDL_DestroyTexture(floor->texture);
    free(floor);
}