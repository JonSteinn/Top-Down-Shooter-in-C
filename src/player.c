#include "player.h"


Player* init_player(SDL_Renderer* renderer, float x, float y) {
    SDL_Surface* surface = IMG_Load("assets/sprites/player.png");
    if (surface == NULL) {
        SDL_Log("Player sprite not found: %s\n", SDL_GetError());
        return NULL;
    }
    
    Player* p = (Player*)malloc(sizeof(Player));
    p->position = (Vector2d*)malloc(sizeof(Vector2d));
    p->velocity = (Vector2d*)malloc(sizeof(Vector2d));
    p->position->x = x;
    p->position->y = y;


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

void destroy_player(Player* player) {
    SDL_DestroyTexture(player->texture);

    free(player->position);
    free(player->velocity);
    free(player);
}