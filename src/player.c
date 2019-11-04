#include "player.h"


Player* init_player(float x, float y) {
    Player* p = (Player*)malloc(sizeof(Player));
    p->position = (Vector2d*)malloc(sizeof(Vector2d));
    p->velocity = (Vector2d*)malloc(sizeof(Vector2d));
    p->position->x = x;
    p->position->y = y;
    return p;
}

void destroy_player(Player* player) {
    free(player->position);
    free(player->velocity);
    free(player);
}