#include "player.h"

/*************
 * Bit masks *
 *************/
// Release all resources
static const uint32_t FREE_ALL = 0xFFFFFFFFu;
// Release SDL surface data
static const uint32_t FREE_SURFACE = 1u<<0;
// Free Player object
static const uint32_t FREE_MEMORY = 1u<<1;
// Release SDL texture data
static const uint32_t FREE_TEXTURE = 1u<<2;

// Path to sprite file
static const char SPRITE_PATH[] = "assets/sprites/player.png";
// Error message when loading image failed
static const char LOAD_IMG_LOG[] = "Player sprite not found: %s\n";
// Error message when fails to create surface
static const char CREATE_TEXTURE_LOG[] = "Could not create texture from surface: %s\n";
// Error message when texture query fails
static const char QUERY_TEXTURE_LOG[] = "Could not query texture: %s\n";
// The scaling factor for all movement directions
static const float PLAYER_SPEED = 0.2f;

/**
 * Function:
 *  __move_left
 *
 * Purpose:
 *  Move the player to the left.
 *
 * Parameters:
 *  - player:
 *      The player object.
 *  - dt:
 *      Delta time.
 *
 * Returns:
 *  Nothing
 */
static void __move_left(Player* player, float dt);

/**
 * Function:
 *  __move_right
 *
 * Purpose:
 *  Move the player to the right.
 *
 * Parameters:
 *  - player:
 *      The player object.
 *  - dt:
 *      Delta time.
 *  - w:
 *      The width of the window.
 *
 * Returns:
 *  Nothing.
 */
static void __move_right(Player* player, float dt, int32_t w);

/**
 * Function:
 *  __move_up
 *
 * Purpose:
 *  Move the player to the up.
 *
 * Parameters:
 *  - player:
 *      The player object.
 *  - dt:
 *      Delta time.
 *
 * Returns:
 *  Nothing.
 */
static void __move_up(Player* player, float dt);

/**
 * Function:
 *  __move_down
 *
 * Purpose:
 *  Move the player to the down.
 *
 * Parameters:
 *  - player:
 *      The player object.
 *  - dt:
 *      Delta time.
 *  - h:
 *      The height of the window.
 *
 * Returns:
 *  Nothing.
 */
static void __move_down(Player* player, float dt, int32_t h);

/**
 * Function:
 *  __rotate
 *
 * Purpose:
 *  Rotate the player towards the mouse.
 *
 * Parameters:
 *  - player:
 *      The player object.
 *  - gevts:
 *      The game events that occured.
 *
 * Returns:
 *  Nothing.
 */
static void __rotate(Player* player, GameEvents* gevts);

/**
 * Function:
 *  __update_collider
 *
 * Purpose:
 *  Update the center position of the collider object around
 *  the player.
 *
 * Parameters:
 *  - player:
 *      The player object.
 *
 * Returns:
 *  Nothing.
 */
static void __update_collider(Player* player);

/**
 * Function:
 *  __destroy
 *
 * Purpose:
 *  Release resources of the Player object.
 *
 * Parameters:
 *  - player:
 *      The player object.
 *  - surface:
 *      SDL_Surface that only lives in function scopes.
 *  - mask:
 *      A mask to choose which resources are destroyed.
 *      FREE_ALL
 *      FREE_SURFACE
 *      FREE_MEMORY
 *      FREE_TEXTURE
 *
 * Returns:
 *  Nothing.
 */
static void __destroy(Player* player, SDL_Surface* surface, uint32_t mask);

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
 *  - player
 *      The player object.
 *
 * Returns:
 *  true if successful, false otherwise.
 */
static bool __create_texture(SDL_Renderer* renderer, SDL_Surface* surface, Player* player);

/**
 * Function:
 *  __query_texture
 *
 * Purpose:
 *  Query texture for its dimension.
 *
 * Parameters:
 *  - player:
 *      The Player object.
 *  - surface:
 *      A structure that contains a collection of pixels used in software blitting.
 *
 * Returns:
 *  true if successful, false otherwise.
 */
static bool __query_texture(Player* player, SDL_Surface* surface);

/**
 * We begin by loading image and if that fails, we stop there.
 * At any point when the initialization fails, we must release
 * previously allocated resources at that point. SDL_Surface
 * does not need to be stored.
 */
Player* init_player(SDL_Renderer* renderer, float x, float y) {
    SDL_Surface* surface = IMG_Load(SPRITE_PATH);
    if (surface == NULL) {
        SDL_Log(LOAD_IMG_LOG, SDL_GetError());
        return NULL;
    }

    Player* p = (Player*)malloc(sizeof(Player));

    if (!__create_texture(renderer, surface, p)) return NULL;
    if (!__query_texture(p, surface)) return NULL;

    // The lesser of the two.
    p->collider.radius = (p->texture_width < p->texture_height ? p->texture_width : p->texture_height) >> 1;
    p->position = (Point2d){x, y};

    SDL_FreeSurface(surface);

    return p;
}

/**
 * Move the player in any requested direction as long as he
 * will not leave the screen and then rotate him towards
 * the mouse. Checks for wall collision.
 */
void update_player(Player* player, GameEvents* gevts, float dt, int32_t w, int32_t h) {
    if (gevts->move_left) __move_left(player, dt);
    if (gevts->move_right) __move_right(player, dt, w);
    if (gevts->move_up) __move_up(player, dt);
    if (gevts->move_down) __move_down(player, dt, h);
    __rotate(player, gevts);
    __update_collider(player);
}

/**
 * Convert the player position into integers before
 * rendering. The rotation is clockwise.
 */
void draw_player(SDL_Renderer* renderer,Player* player) {
    SDL_Rect rect = {
        (int)player->position.x,
        (int)player->position.y,
        player->texture_width,
        player->texture_height
    };
    SDL_RenderCopyEx(
        renderer,
        player->texture,
        NULL,
        &rect,
        rad_to_deg(player->rotation),
        NULL,
        SDL_FLIP_NONE
    );
}

/**
 * Does not need to release the surface, hence we
 * do not use the FREE_ALL mask.
 */
void destroy_player(Player* player) {
    __destroy(player, NULL, FREE_TEXTURE | FREE_MEMORY);
}

/**
 * Moves left if we are not too close to the left border.
 */
static void __move_left(Player* player, float dt) {
    if (player->collider.center.x - player->collider.radius >= 0) {
        player->position.x -= PLAYER_SPEED * dt;
    }
}

/**
 * Moves right if we are not too close to the right border.
 */
static void __move_right(Player* player, float dt, int32_t w) {
    if (player->collider.center.x + player->collider.radius <= w) {
        player->position.x += PLAYER_SPEED * dt;
    }
}

/**
 * Moves up if we are not too close to the upper border.
 */
static void __move_up(Player* player, float dt) {
    if (player->collider.center.y - player->collider.radius >= 0) {
        player->position.y -= PLAYER_SPEED * dt;
    }
}

/**
 * Moves down if we are not too close to the lower border.
 */
static void __move_down(Player* player, float dt, int32_t h) {
    if (player->collider.center.y + player->collider.radius <= h) {
        player->position.y += PLAYER_SPEED * dt;
    }
}

/**
 * Let P be the position of the player and M the position of the mouse.
 * Then the angle between the vector v=[P to M] and u=[1,0] is the angle
 * we are looking for. Note the u=[1,0] is chosen because the sprite
 * faces east with no rotation.
 *
 * cos(x) = (v*u) / (|v|*|u|)
 *        = (1*v_x + 0*v_y) / ( sqrt(1*1+0+0) + sqrt(v_x*v_x + v_y*v_y))
 *        = v_x / (sqrt(1) * sqrt(|v|*|v|))
 *        = v_x / sqrt(|v|*|v|)
 * => x = arccos(v_x / sqrt(|v| * |v|))
 *
 * Since arccos always chooses the lesser of angles betwee two vectorss
 * (if there is a 120° angle between some vectors, there is also a 240°
 * angle between them in the other direction), we multiply with the sign
 * of v_y to rotate in the correct direction.
 */
static void __rotate(Player* player, GameEvents* gevts) {
    Vector2d d = {
        gevts->mouseX - player->position.x,
        gevts->mouseY - player->position.y
    };
    player->rotation = sign(d.y) * fast_acos(d.x * carmack_inverse_sqrt(length_squared(&d)));
}

/**
 * Our image for the player is something like:
 * ---------
 * | A | B |
 * ---------
 * where A is the human and B is the gun. We don't wish
 * for the collider to cover the gun so we must transform
 * the center of the collider in the opposite direction the
 * player faces by a quarter of the width of the texture.
 */
static void __update_collider(Player* player) {
    int32_t scale = player->texture_width / 4;
    player->collider.center.x = (player->position.x + player->texture_width / 2)
        - SDL_cosf(player->rotation) * scale; // TODO: fast_cos
    player->collider.center.y = (player->position.y + player->texture_height / 2)
        - SDL_sinf(player->rotation) * scale; // TODO: fast_sin
}

/**
 * Check each resources against mask before releasing.
 */
static void __destroy(Player* player, SDL_Surface* surface, uint32_t mask) {
    if (mask & FREE_SURFACE) SDL_FreeSurface(surface);
    if (mask & FREE_TEXTURE) SDL_DestroyTexture(player->texture);
    if (mask & FREE_MEMORY) free(player);
}

/**
 * Releases the surface resources and Player memory if we fail
 * to create the texture.
 */
static bool __create_texture(SDL_Renderer* renderer, SDL_Surface* surface, Player* player) {
    player->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (player->texture == NULL) {
        SDL_Log(CREATE_TEXTURE_LOG, SDL_GetError());
        __destroy(player, surface, FREE_SURFACE | FREE_MEMORY);
        return false;
    }
    return true;
}

/**
 * Releases the surface and texture resources and Player memory
 * if we fail to query the texture.
 */
static bool __query_texture(Player* player, SDL_Surface* surface) {
    if (SDL_QueryTexture(player->texture, NULL, NULL, &player->texture_width, &player->texture_height) < 0) {
        SDL_Log(QUERY_TEXTURE_LOG, SDL_GetError());
        __destroy(player, surface, FREE_ALL);
        return false;
    }
    return true;
}