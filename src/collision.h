#ifndef a3Km81rPm0_COLLISION_H
#define a3Km81rPm0_COLLISION_H

#include <stdbool.h>

#include "gmath.h"

typedef struct {
    Point2d center;
    float   radius;
} Collider;

bool collide(Collider* c1, Collider* c2);


#endif