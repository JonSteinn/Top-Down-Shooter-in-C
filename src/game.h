#ifndef dR4wsjUCND_GAME_H
#define dR4wsjUCND_GAME_H

#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* window;
} Game;

Game* init_game(int32_t argc, char** args);
void start_game(Game* game);
void destroy_game(Game* game);

#endif