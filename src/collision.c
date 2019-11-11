#include "collision.h"


bool collide(Collider* c1, Collider* c2) {
    Vector2d c1c2 = {c2->center.x - c1->center.x, c2->center.y - c1->center.y };
    float lenSq = length_squared(&c1c2);
    float radSq = c1->radius + c2->radius;
    radSq *= radSq;
    return lenSq < radSq;
}