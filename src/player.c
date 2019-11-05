#include "player.h"


Player* init_player(SDL_Renderer* renderer, float x, float y) {
    SDL_Surface* surface = IMG_Load("assets/sprites/player.png");
    if (surface == NULL) {
        SDL_Log("Player sprite not found: %s\n", SDL_GetError());
        return NULL;
    }
    
    Player* p = (Player*)malloc(sizeof(Player));
    p->position = (Point2d*)malloc(sizeof(Point2d));
    p->position->x = x;
    p->position->y = y;
    p->rotation = -90;


    p->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (surface == NULL) {
        SDL_Log("Could not create texture from surface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(p);
        return NULL;   
    }

    SDL_FreeSurface(surface);
    
    return p;
}


void update_player(Player* player, GameEvents* gevts, float dt, int32_t w, int32_t h) {

    if (gevts->move_left) {
        player->position->x -= 0.15f * dt;
        if (player->position->x < 0) player->position->x = 0.0f;
    }

    if (gevts->move_right) {
        player->position->x += 0.15f * dt;
        if (player->position->x + 54 > w) {
            player->position->x = w - 54;
        }
    }
    
    if (gevts->move_up) {
        player->position->y -= 0.15f * dt;
        if (player->position->y < 0) player->position->y = 0.0f;
    }
    
    if (gevts->move_down) {
        player->position->y += 0.15f * dt;
        if (player->position->y + 32 > h) {
            player->position->y = h - 32;
        } 
    }

    Vector2d d = {gevts->mouseX - player->position->x, gevts->mouseY - player->position->y};
    player->rotation = sign(d.y) * rad_to_deg(fast_acos(d.x * carmack_inverse_sqrt(length_squared(&d))));
}

void draw_player(SDL_Renderer* renderer,Player* player) {
    SDL_Rect rect = { (int)player->position->x, (int)player->position->y, 54, 32 } ;
    SDL_RenderCopyEx(renderer, player->texture, NULL, &rect, player->rotation, NULL, SDL_FLIP_NONE);
}

void destroy_player(Player* player) {
    SDL_DestroyTexture(player->texture);

    free(player->position);
    free(player);
}