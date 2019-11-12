#ifndef a3Km81rPm0_COLLISION_H
#define a3Km81rPm0_COLLISION_H

#include <stdbool.h>

#include "gmath.h"
#include "enemies.h"

typedef struct {
    Point2d center;
    float   radius;
} Collider;

bool player_enemy_collision(Collider* p_collider, Enemies* enemies);


#endif