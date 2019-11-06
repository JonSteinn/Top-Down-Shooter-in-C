#include "enemies.h"

static void __init_enemy(Enemy* enemy);


Enemies* init_enemies(SDL_Renderer* renderer, int32_t max_enemies) {
    SDL_Surface* surface = IMG_Load("assets/sprites/enemy.png");
    if (surface == NULL) {
        SDL_Log("Enemy sprite not found: %s\n", SDL_GetError());
        return NULL;
    }

    Enemies* e = (Enemies*)malloc(sizeof(Enemies));
    e->enemies = (Enemy*)malloc(sizeof(Enemy) * max_enemies);

    e->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (e->texture == NULL) {
        SDL_Log("Could not create texture from surface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(e->enemies);
        free(e);
        return NULL;   
    }

    SDL_FreeSurface(surface);

    // Done with: http://www.spritecow.com/ 
    SDL_Rect rects[6] = {
        { 36, 22, 61, 62 },
        { 114, 23, 60, 62 },
        { 189, 26, 58, 62 },
        { 41, 98, 59, 61 },
        { 112, 99, 60, 61 },
        { 186, 101, 58, 61 }
    };
    memcpy(e->texture_states, rects, sizeof(rects));

    e->max_enemies = max_enemies;

    for (int32_t i = 0; i < e->max_enemies; i++) {
        __init_enemy(e->enemies + i);
    }

    return e;
}

void update_enemies(Enemies* enemies, float dt) {
    for (int32_t i = 0; i < enemies->max_enemies; i++) {
        enemies->enemies[i].statef += dt * 0.01f;
        if (enemies->enemies[i].statef >= 6) enemies->enemies[i].statef -= (int)enemies->enemies[i].statef;
        enemies->enemies[i].state = (int)enemies->enemies[i].statef;
    }
}

void draw_enemies(SDL_Renderer* renderer, Enemies* enemies) {
    for (int32_t i = 0; i < enemies->max_enemies; i++) {
        Enemy e = enemies->enemies[i];
        SDL_Rect rect = { e.position.x, e.position.y,  50, 50 } ;
        SDL_RenderCopyEx(renderer, enemies->texture, &enemies->texture_states[e.state], &rect, e.rotation, NULL, SDL_FLIP_NONE);
    }
}

void destroy_enemies(Enemies* enemies) {
    free(enemies->enemies);
    free(enemies);
}

static void __init_enemy(Enemy* enemy) {
    enemy->health = 2;
    enemy->position.x = 50; // TODO: Random, outside win
    enemy->position.y = 50; // TODO: Random, outside win
    enemy->rotation = 90;
    enemy->state = 0;
    enemy->statef = 0.0f;
    enemy->velocity.x = 0;
    enemy->velocity.y = 0;
}