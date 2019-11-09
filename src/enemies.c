#include "enemies.h"

static void __init_enemy(Enemy* enemy, int32_t w, int32_t h);
static void __update_enemy(Enemy* enemy, float dt, Point2d* p_pos);


Enemies* init_enemies(SDL_Renderer* renderer, int32_t max_enemies, int32_t w, int32_t h) {
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
    e->texture_states[0] = (SDL_Rect){ 36, 22, 61, 62 };
    e->texture_states[1] = (SDL_Rect){ 114, 23, 60, 62 };
    e->texture_states[2] = (SDL_Rect){ 189, 26, 58, 62 };
    e->texture_states[3] = (SDL_Rect){ 41, 98, 59, 61 };
    e->texture_states[4] = (SDL_Rect){ 112, 99, 60, 61 };
    e->texture_states[5] = (SDL_Rect){ 186, 101, 58, 61 };
    e->max_enemies = max_enemies;

    for (int32_t i = 0; i < e->max_enemies; i++) {
        __init_enemy(e->enemies + i, w, h);
    }

    return e;
}

void update_enemies(Enemies* enemies, float dt, Point2d* p_pos) {
    for (int32_t i = 0; i < enemies->max_enemies; i++) {
        __update_enemy(enemies->enemies + i, dt, p_pos);
    }
}

void draw_enemies(SDL_Renderer* renderer, Enemies* enemies) {
    for (int32_t i = 0; i < enemies->max_enemies; i++) {
        Enemy e = enemies->enemies[i];
        SDL_Rect rect = { e.position.x, e.position.y,  50, 50 } ;
        SDL_RenderCopyEx(renderer, enemies->texture, &enemies->texture_states[(int)e.state], &rect, e.rotation, NULL, SDL_FLIP_NONE);
    }
}

void destroy_enemies(Enemies* enemies) {
    free(enemies->enemies);
    free(enemies);
}

void __init_enemy(Enemy* enemy, int32_t w, int32_t h) {
    if (rand() % 2) {
        enemy->position.x = (rand() % (w+1000) - 500);
        if (enemy->position.x >= -50 && enemy->position.x <= w + 50) {
            enemy->position.y = rand() % 2 ? -(100 + (rand() % 500)) : h + (100 + (rand() % 500));
        } else {
            enemy->position.y = (rand() % (h+1000) - 500);
        }
    } else {
        enemy->position.y = (rand() % (h+1000) - 500);
        if (enemy->position.y >= -50 && enemy->position.y <= h + 50) {
            enemy->position.x = rand() % 2 ? -(100 + (rand() % 500)) : w + (100 + (rand() % 500));
        } else {
            enemy->position.x = (rand() % (w+1000) - 500);
        }
    }

    enemy->rotation = 90;
    enemy->state = 0.0f;
}


void __update_enemy(Enemy* enemy, float dt, Point2d* p_pos) {
    enemy->state += dt * 0.01f;
    if (enemy->state >= 6.0f) enemy->state -= (float)((int)enemy->state);

    float dx = p_pos->x - enemy->position.x;
    float dy = p_pos->y - enemy->position.y;
    float norm_factor = carmack_inverse_sqrt(dx*dx+dy*dy);

    enemy->position.x += dx * norm_factor * dt * 0.05f;
    enemy->position.y += dy * norm_factor * dt * 0.05f;

    enemy->rotation = sign(-dx) * rad_to_deg(fast_acos(dy * norm_factor)) + 180;
}