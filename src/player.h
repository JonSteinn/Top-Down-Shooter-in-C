#ifndef jPERNJXbbF_PLAYER_H
#define jPERNJXbbF_PLAYER_H

#include <stdlib.h>
#include "gmath.h"

typedef struct {
    Vector2d* position;
    Vector2d* velocity;
} Player;

Player* init_player(float x, float y);
void destroy_player(Player* player);

#endif