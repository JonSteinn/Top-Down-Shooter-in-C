#ifndef M3cvCxcSjG_ENEMIES_H
#define M3cvCxcSjG_ENEMIES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "gmath.h"

typedef struct {
    Point2d     position;
    Vector2d    velocity;
    float       rotation;
    float       statef;
    int32_t     state;
    int32_t     health;
} Enemy;

typedef struct {
    SDL_Texture*    texture;
    SDL_Rect        texture_states[6];
    Enemy*          enemies;
    int32_t         max_enemies;
} Enemies;


Enemies* init_enemies(SDL_Renderer* renderer, int32_t max_enemies);

void update_enemies(Enemies* enemies, float dt);
void draw_enemies(SDL_Renderer* renderer, Enemies* enemies);

void destroy_enemies(Enemies* enemies);

#endif