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


static void __move_left(Player* player, float dt);
static void __move_right(Player* player, float dt, int32_t w);
static void __move_up(Player* player, float dt);
static void __move_down(Player* player, float dt, int32_t h);
static void __rotate(Player* player, GameEvents* gevts);
static void __destroy(Player* player, SDL_Surface* surface, uint32_t mask);
static bool __create_texture(SDL_Renderer* renderer, SDL_Surface* surface, Player* play);
static bool __query_texture(Player* player, SDL_Surface* surface);

/**
 * 
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
    
    p->collision_circumference = p->texture_width < p->texture_height ? p->texture_width : p->texture_height;
    p->position = (Point2d){x, y};

    SDL_FreeSurface(surface);
    
    return p;
}

/**
 * 
 */
void update_player(Player* player, GameEvents* gevts, float dt, int32_t w, int32_t h) {
    if (gevts->move_left) __move_left(player, dt);
    if (gevts->move_right) __move_right(player, dt, w);
    if (gevts->move_up) __move_up(player, dt);
    if (gevts->move_down) __move_down(player, dt, h);
    __rotate(player, gevts);
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
    SDL_RenderCopyEx(renderer, player->texture, NULL, &rect, player->rotation, NULL, SDL_FLIP_NONE);
}

/**
 * 
 */
void destroy_player(Player* player) {
    __destroy(player, NULL, FREE_TEXTURE | FREE_MEMORY);
}

/**
 * 
 */
static void __move_left(Player* player, float dt) {
    player->position.x -= PLAYER_SPEED * dt;
    if (player->position.x < 0) player->position.x = 0.0f;
}

/**
 * 
 */
static void __move_right(Player* player, float dt, int32_t w) {
    player->position.x += PLAYER_SPEED * dt;
    if (player->position.x + player->collision_circumference > w) {
        player->position.x = w - player->collision_circumference;
    }
}

/**
 * 
 */
static void __move_up(Player* player, float dt) {
    player->position.y -= PLAYER_SPEED * dt;
    if (player->position.y < 0) player->position.y = 0.0f;
}

/**
 * 
 */
static void __move_down(Player* player, float dt, int32_t h) {
    player->position.y += PLAYER_SPEED * dt;
    if (player->position.y + player->collision_circumference > h) {
        player->position.y = h - player->collision_circumference;
    } 
}

/**
 * 
 */
static void __rotate(Player* player, GameEvents* gevts) {
    Vector2d d = {gevts->mouseX - player->position.x, gevts->mouseY - player->position.y};
    player->rotation = sign(d.y) * rad_to_deg(fast_acos(d.x * carmack_inverse_sqrt(length_squared(&d))));
}

/**
 * 
 */
static void __destroy(Player* player, SDL_Surface* surface, uint32_t mask) {
    if (mask & FREE_SURFACE) SDL_FreeSurface(surface);
    if (mask & FREE_TEXTURE) SDL_DestroyTexture(player->texture);
    if (mask & FREE_MEMORY) free(player);
}

/**
 * 
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
 * 
 */
static bool __query_texture(Player* player, SDL_Surface* surface) {
    if (SDL_QueryTexture(player->texture, NULL, NULL, &player->texture_width, &player->texture_height) < 0) {
        SDL_Log(QUERY_TEXTURE_LOG, SDL_GetError());
        __destroy(player, surface, FREE_ALL);
        return false;   
    }
    return true;
}