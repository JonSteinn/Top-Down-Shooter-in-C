#ifndef dR4wsjUCND_GAME_H
#define dR4wsjUCND_GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window*     window;
    SDL_Renderer*   renderer;

    bool            is_running;
    
} Game;

Game* init_game(int32_t argc, char** args);
void start_game(Game* game);
void destroy_game(Game* game);

#endif