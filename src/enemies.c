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
// How fast the enemy animates
static const float ENEMY_ANIMATION_SPEED = 0.01f;
// How fast the enemy walks
static const float ENEMY_WALKING_SPEED = 0.05f;
// Enemy size
static const int32_t ENEMY_SIZE = 40;

/**
 * Function:
 *  __alloc_and_set_enemies
 *
 * Purpose:
 *  Allocate memory for the Enemies object and initialize
 *  some of its values.
 *
 * Parameters:
 *  max_enemies:
 *      How large the array of enemies should be.
 *
 * Returns:
 *  The Enemies object allocated.
 */
static Enemies* __alloc_and_set_enemies(int32_t max_enemies);

/**
 * Function:
 *  __create_texture
 *
 * Purpose:
 *  Create a texture from an existing surface.
 *
 * Parameters:
 *  - renderer:
 *      A structure that contains a rendering state.
 *  - surface:
 *      A structure that contains a collection of pixels used in software blitting.
 *  - enemies:
 *      The Enemies object.
 *
 * Returns:
 *  true if successful, false otherwise.
 */
static bool __create_texture(SDL_Renderer* renderer, SDL_Surface* surface, Enemies* enemies);

/**
 * Function:
 *  __destroy
 *
 * Purpose:
 *  Release resources of the Enemies object.
 *
 * Parameters:
 *  - enemies:
 *      The Enemies object.
 *  - surface:
 *      SDL_Surface that only lives in function scopes.
 *  - mask:
 *      A mask to choose which resources are destroyed.
 *      FREE_SURFACE
 *      FREE_MEMORY
 *      FREE_TEXTURE
 *      FREE_TEXTURE
 *
 * Returns:
 *  Nothing.
 */
static void __destroy(Enemies* enemies, SDL_Surface* surface, uint32_t mask);

/**
 * Function:
 *  __init_enemy
 *
 * Purpose:
 *  Initialize the enemy's position.
 *
 * Parameters:
 *  - enemy:
 *      The Enemy object.
 *  - w:
 *      The width of the window.
 *  - h:
 *      The height of the window.
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
 *  Choose x-coordinate for enemy randomly first, then y based
 *  on the choice of x so they spawn outside the window.
 *
 * Parameters:
 *  - enemy:
 *      The Enemy object.
 *  - w:
 *      The width of the window.
 *  - h:
 *      The height of the window.
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
 *  Choose y-coordinate for enemy randomly first, then x based
 *  on the choice of y so they spawn outside the window.
 *
 * Parameters:
 *  - enemy:
 *      The Enemy object.
 *  - w:
 *      The width of the window.
 *  - h:
 *      The height of the window.
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
 *  Update enemy's animation, position and rotation base
 *  on time and player position.
 *
 * Parameters:
 *  - enemy:
 *      The Enemy object.
 *  - dt:
 *      Delta time.
 *  - p_pos:
 *      The position of the player.
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
 *  Draw the enemy in his current animation state.
 *
 * Parameters:
 *  - renderer:
 *      A structure that contains a rendering state.
 *  - enemies:
 *      The Enemies object.
 *  - enemy_index:
 *      The index of the enemy to update in the enemy array.
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
 * Calls draw for each enemy, given they are visible.
 */
void draw_enemies(SDL_Renderer* renderer, Enemies* enemies, int32_t w, int32_t h) {
    for (int32_t i = 0; i < enemies->max_enemies; i++) {
        bool visible = enemies->enemies[i].position.x > -ENEMY_SIZE
            && enemies->enemies[i].position.x < w + ENEMY_SIZE
            && enemies->enemies[i].position.y > -ENEMY_SIZE
            && enemies->enemies[i].position.y < h + ENEMY_SIZE;
        if (visible) __draw_enemy(renderer, enemies, i);
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
 * Allocate memory for Enemies and its Enemy array. Set the
 * texture states array to the rectangles surrounding each
 * image within the sprite sheet.
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
    e->collision_radius = 0.9f * ENEMY_SIZE/2.0f;

    return e;
}

/**
 * Releases the surface resources and Enemies memory if we fail
 * to create the texture.
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
 * Check each resources against mask before releasing.
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
 * Initialize an enemy to a random position within the world,
 * outside the view of the player but not too far off.
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
 * Choose horizontal position of enemy first and then the
 * vertical position based on that, so they always spawn
 * outside the window.
 */
static void __pick_x_first(Enemy* enemy, int32_t w, int32_t h) {
    // Pick x uniform from [-500,w+500]
    enemy->position.x = (rand() % (w+1000) - 500);

    // If x is within the window boundary (with a little leeway)
    if (enemy->position.x >= -ENEMY_SIZE && enemy->position.x <= w + ENEMY_SIZE) {
        // Pick y to be outside the window boundary
        enemy->position.y = rand() % 2 ? -(100 + (rand() % 500)) : h + (100 + (rand() % 500));
    } else {
        // Pick y uniformly from [-500,h+500]
        enemy->position.y = (rand() % (h+1000) - 500);
    }
}

/**
 * Choose vertical position of enemy first and then the
 * horizontal position based on that, so they always spawn
 * outside the window.
 */
static void __pick_y_first(Enemy* enemy, int32_t w, int32_t h) {
    // Pick y uniformly from [-500,h+500]
    enemy->position.y = (rand() % (h+1000) - 500);

    // If y is within the window boundary (with a little leeway)
    if (enemy->position.y >= -ENEMY_SIZE && enemy->position.y <= h + ENEMY_SIZE) {
        // Pick x to be outside the window boundary
        enemy->position.x = rand() % 2 ? -(100 + (rand() % 500)) : w + (100 + (rand() % 500));
    } else {
        // Pick y uniformly from [-500,h+500]
        enemy->position.x = (rand() % (w+1000) - 500);
    }
}

/**
 * Update animation state and move the enemy a little closer to the player,
 * also rotate the texture to be facing the player.
 */
static void __update_enemy(Enemy* enemy, float dt, Point2d* p_pos) {
    // Animate
    enemy->state += dt * ENEMY_ANIMATION_SPEED;
    if (enemy->state >= 6.0f) enemy->state -= (float)((int)enemy->state);

    // Math
    Vector2d e_to_p = {p_pos->x - enemy->position.x, p_pos->y - enemy->position.y};
    float norm_factor = carmack_inverse_sqrt(length_squared(&e_to_p));
    float scale = norm_factor * dt * ENEMY_WALKING_SPEED;

    // Move
    enemy->position = (Point2d){enemy->position.x + e_to_p.x * scale, enemy->position.y + e_to_p.y * scale };

    // Rotate
    enemy->rotation = sign(-e_to_p.x) * rad_to_deg(fast_acos(e_to_p.y * norm_factor)) + 180;
}

/**
 * Draw the enemy. The animation state dictates which part
 * of the spritesheet is drawn.
 */
static void __draw_enemy(SDL_Renderer* renderer, Enemies* enemies, int32_t enemy_index) {
    Enemy e = enemies->enemies[enemy_index];
    SDL_Rect rect = { e.position.x, e.position.y,  ENEMY_SIZE, ENEMY_SIZE } ;
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