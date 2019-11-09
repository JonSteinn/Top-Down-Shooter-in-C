#include "enemies.h"

/*************
 * Bit masks *
 *************/
// Release SDL surface data
static const uint32_t FREE_SURFACE = 1u<<0;
// Free Enemies object and its Enemy array
static const uint32_t FREE_MEMORY = 1u<<1;
// Release SDL texture data
static const uint32_t FREE_TEXTURE = 1u<<2;

// Path to sprite file
static const char SPRITE_PATH[] = "assets/sprites/enemy.png";
// Error message when loading image failed
static const char LOAD_IMG_LOG[] = "Enemy sprite not found: %s\n";
// Error message when fails to create surface
static const char CREATE_TEXTURE_LOG[] = "Could not create texture from surface: %s\n";

/**
 * Function:
 *  __alloc_and_set_enemies
 *
 * Purpose:
 *
 * Parameters:
 *
 * Returns:
 */
static Enemies* __alloc_and_set_enemies(int32_t max_enemies);

/**
 * Function:
 *  __create_texture
 *
 * Purpose:
 *
 * Parameters:
 *
 * Returns:
 */
static bool __create_texture(SDL_Renderer* renderer, SDL_Surface* surface, Enemies* enemies);

/**
 * Function:
 *  __destroy
 *
 * Purpose:
 *
 * Parameters:
 *
 * Returns:
 *  Nothing.
 */
static void __destroy(Enemies* player, SDL_Surface* surface, uint32_t mask);

/**
 * Function:
 *  __init_enemy
 *
 * Purpose:
 *
 * Parameters:
 *
 * Returns:
 *  Nothing.
 */
static void __init_enemy(Enemy* enemy, int32_t w, int32_t h);

/**
 * Function:
 *  __pick_x_first
 *
 * Purpose:
 *
 * Parameters:
 *
 * Returns:
 *  Nothing.
 */
static void __pick_x_first(Enemy* enemy, int32_t w, int32_t h);

/**
 * Function:
 *  __pick_y_first
 *
 * Purpose:
 *
 * Parameters:
 *
 * Returns:
 *  Nothing.
 */
static void __pick_y_first(Enemy* enemy, int32_t w, int32_t h);

/**
 * Function:
 *  __update_enemy
 *
 * Purpose:
 *
 * Parameters:
 *
 * Returns:
 *  Nothing.
 */
static void __update_enemy(Enemy* enemy, float dt, Point2d* p_pos);

/**
 * Function:
 *  __draw_enemy
 *
 * Purpose:
 *
 * Parameters:
 *
 * Returns:
 *  Nothing.
 */
static void __draw_enemy(SDL_Renderer* renderer, Enemies* enemies, int32_t enemy_index);

/**
 * We begin by loading image and if that fails, we stop there.
 * At any point when the initialization fails, we must release
 * previously allocated resources at that point. SDL_Surface
 * does not need to be stored. Each enemy is then initalized
 * separately.
 */
Enemies* init_enemies(SDL_Renderer* renderer, int32_t max_enemies, int32_t w, int32_t h) {
    SDL_Surface* surface = IMG_Load(SPRITE_PATH);
    if (surface == NULL) {
        SDL_Log(LOAD_IMG_LOG, SDL_GetError());
        return NULL;
    }

    Enemies* e = __alloc_and_set_enemies(max_enemies);

    if (!__create_texture(renderer, surface, e)) return NULL;

    SDL_FreeSurface(surface);

    for (int32_t i = 0; i < e->max_enemies; i++) {
        __init_enemy(e->enemies + i, w, h);
    }

    return e;
}

/**
 * Calls update for each enemy.
 */
void update_enemies(Enemies* enemies, float dt, Point2d* p_pos) {
    for (int32_t i = 0; i < enemies->max_enemies; i++) {
        __update_enemy(enemies->enemies + i, dt, p_pos);
    }
}

/**
 * Calls draw for each enemy.
 */
void draw_enemies(SDL_Renderer* renderer, Enemies* enemies) {
    for (int32_t i = 0; i < enemies->max_enemies; i++) {
        __draw_enemy(renderer, enemies, i);
    }
}

/**
 * Releases all resources related to enemies that have
 * been stored in the Enemies object.
 */
void destroy_enemies(Enemies* enemies) {
    __destroy(enemies, NULL, FREE_TEXTURE | FREE_MEMORY);
}

/**
 *
 */
static Enemies* __alloc_and_set_enemies(int32_t max_enemies) {
    Enemies* e = (Enemies*)malloc(sizeof(Enemies));
    e->enemies = (Enemy*)malloc(sizeof(Enemy) * max_enemies);

    // Done with: http://www.spritecow.com/
    e->texture_states[0] = (SDL_Rect){ 36, 22, 61, 62 };
    e->texture_states[1] = (SDL_Rect){ 114, 23, 60, 62 };
    e->texture_states[2] = (SDL_Rect){ 189, 26, 58, 62 };
    e->texture_states[3] = (SDL_Rect){ 41, 98, 59, 61 };
    e->texture_states[4] = (SDL_Rect){ 112, 99, 60, 61 };
    e->texture_states[5] = (SDL_Rect){ 186, 101, 58, 61 };

    e->max_enemies = max_enemies;

    return e;
}

/**
 *
 */
static bool __create_texture(SDL_Renderer* renderer, SDL_Surface* surface, Enemies* enemies) {
    enemies->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (enemies->texture == NULL) {
        SDL_Log(CREATE_TEXTURE_LOG, SDL_GetError());
        __destroy(enemies, surface, FREE_SURFACE | FREE_MEMORY);
        return false;
    }
    return true;
}

/**
 *
 */
static void __destroy(Enemies* enemies, SDL_Surface* surface, uint32_t mask) {
    if (FREE_SURFACE & mask) SDL_FreeSurface(surface);
    if (FREE_TEXTURE & mask) SDL_DestroyTexture(enemies->texture);
    if (FREE_MEMORY & mask) {
        free(enemies->enemies);
        free(enemies);
    }
}

/**
 *
 */
static void __init_enemy(Enemy* enemy, int32_t w, int32_t h) {
    if (rand() % 2) {
        __pick_x_first(enemy, w, h);
    } else {
        __pick_y_first(enemy, w, h);
    }
    enemy->state = 0.0f;
}

/**
 *
 */
static void __pick_x_first(Enemy* enemy, int32_t w, int32_t h) {
    enemy->position.x = (rand() % (w+1000) - 500);
    if (enemy->position.x >= -50 && enemy->position.x <= w + 50) {
        enemy->position.y = rand() % 2 ? -(100 + (rand() % 500)) : h + (100 + (rand() % 500));
    } else {
        enemy->position.y = (rand() % (h+1000) - 500);
    }
}

/**
 *
 */
static void __pick_y_first(Enemy* enemy, int32_t w, int32_t h) {
    enemy->position.y = (rand() % (h+1000) - 500);
    if (enemy->position.y >= -50 && enemy->position.y <= h + 50) {
        enemy->position.x = rand() % 2 ? -(100 + (rand() % 500)) : w + (100 + (rand() % 500));
    } else {
        enemy->position.x = (rand() % (w+1000) - 500);
    }
}

/**
 *
 */
static void __update_enemy(Enemy* enemy, float dt, Point2d* p_pos) {
    // Animate
    enemy->state += dt * 0.01f;
    if (enemy->state >= 6.0f) enemy->state -= (float)((int)enemy->state);

    // Math
    Vector2d e_to_p = {p_pos->x - enemy->position.x, p_pos->y - enemy->position.y};
    float norm_factor = carmack_inverse_sqrt(length_squared(&e_to_p));
    float scale = norm_factor * dt * 0.05f;

    // Move
    enemy->position = (Point2d){enemy->position.x + e_to_p.x * scale, enemy->position.y + e_to_p.y * scale };

    // Rotate
    enemy->rotation = sign(-e_to_p.x) * rad_to_deg(fast_acos(e_to_p.y * norm_factor)) + 180;
}

/**
 *
 */
static void __draw_enemy(SDL_Renderer* renderer, Enemies* enemies, int32_t enemy_index) {
    Enemy e = enemies->enemies[enemy_index];
    SDL_Rect rect = { e.position.x, e.position.y,  50, 50 } ;
    SDL_RenderCopyEx(
        renderer,
        enemies->texture,
        &enemies->texture_states[(int)e.state],
        &rect,
        e.rotation,
        NULL,
        SDL_FLIP_NONE
    );
}