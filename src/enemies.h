#ifndef M3cvCxcSjG_ENEMIES_H
#define M3cvCxcSjG_ENEMIES_H

#define MAX_ENEMIES 20

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include "gmath.h"

typedef struct {
    Point2d*    position;
    Vector2d*   velocity;
} Enemy;

typedef struct {
    Enemy enemies[MAX_ENEMIES];
} Enemies;

#endif