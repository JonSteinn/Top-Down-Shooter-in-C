#ifndef jPERNJXbbF_PLAYER_H
#define jPERNJXbbF_PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include "gmath.h"

typedef struct {
    Point2d*        position;
    float           rotation;

    SDL_Texture*    texture;
} Player;

Player* init_player(SDL_Renderer* renderer, float x, float y);
void draw_player(SDL_Renderer* renderer, Player* player);
void destroy_player(Player* player);

#endif