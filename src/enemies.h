#ifndef M3cvCxcSjG_ENEMIES_H
#define M3cvCxcSjG_ENEMIES_H

#define MAX_ENEMIES 20

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdlib.h>
#include "gmath.h"

typedef struct {
    Point2d*    position;
    Vector2d*   velocity;
    float       rotation;
    int32_t     state;
    int32_t     health;
} Enemy;

typedef struct {
    SDL_Texture*    texture;
    Enemy*          enemies;
    int32_t         max_enemies;
} Enemies;


Enemies* init_enemies(SDL_Renderer* renderer, int32_t max_enemies);
void destroy_enemies(Enemies* enemies);

#endif