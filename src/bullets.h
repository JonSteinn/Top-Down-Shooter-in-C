#ifndef G4v5mJEDru_BULLETS_H
#define G4v5mJEDru_BULLETS_H

#include "gmath.h"
#include "list.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    Point2d     position;
    Vector2d    direction;
    float       rotation;
} Bullet;

typedef struct {
    List*           bullets;
    SDL_Texture*    texture;
} Bullets;

Bullets* init_bullets();
void destroy_bullets(Bullets* bullets);


#endif