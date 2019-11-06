#include "enemies.h"


Enemies* init_enemies(SDL_Renderer* renderer, int32_t max_enemies) {
    SDL_Surface* surface = IMG_Load("assets/sprites/enemy.png");
    if (surface == NULL) {
        SDL_Log("Enemy sprite not found: %s\n", SDL_GetError());
        return NULL;
    }

    Enemies* e = (Enemies*)malloc(sizeof(Enemies));
    e->enemies = (Enemy*)malloc(sizeof(Enemy) * max_enemies);
    e->max_enemies = max_enemies;

    e->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (e->texture == NULL) {
        SDL_Log("Could not create texture from surface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(e->enemies);
        free(e);
        return NULL;   
    }

    SDL_FreeSurface(surface);

    return e;
}

void destroy_enemies(Enemies* enemies) {
    free(enemies->enemies);
    free(enemies);
}