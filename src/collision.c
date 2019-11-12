#include "collision.h"

static bool __collide(Collider* c1, Collider* c2);
static bool __collide(Collider* c1, Collider* c2) {
    Vector2d c1c2 = {c2->center.x - c1->center.x, c2->center.y - c1->center.y };
    float lenSq = length_squared(&c1c2);

    float radSq = c1->radius + c2->radius;
    radSq *= radSq;

    return lenSq < radSq;
}


bool player_enemy_collision(Collider* p_collider, Enemies* enemies) {
    Collider e_collider;
    e_collider.radius = enemies->collision_radius;
    for (int32_t i = 0; i < enemies->max_enemies; i++) {
        e_collider.center.x = enemies->enemies[i].position.x + e_collider.radius;
        e_collider.center.y = enemies->enemies[i].position.y + e_collider.radius;
        if (__collide(p_collider, &e_collider)) return true;
    }
    return false;
}