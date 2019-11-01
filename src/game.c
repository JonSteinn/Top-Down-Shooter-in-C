#include "game.h"

#define UNUSED(x) (void)x // TODO: remove

static void __process_time(Game* game);
static void __process_events(Game* game);
static void __update(Game* game);
static void __render(Game* game);

Game* init_game(int32_t argc, char** args) {
    UNUSED(argc);UNUSED(args); // TODO: remove

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    Game* game = (Game*)malloc(sizeof(Game));
    game->is_running = true;
    
    game->window = SDL_CreateWindow(
        "MyWindow",                 // window title
        SDL_WINDOWPOS_UNDEFINED,    // initial x position
        SDL_WINDOWPOS_UNDEFINED,    // initial y position
        800,                        // width, in pixels
        600,                        // height, in pixels
        SDL_WINDOW_OPENGL           // flags
    );
    if (game->window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        free(game);
        return NULL;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);      
        free(game);
        return NULL;
    }

    return game;
}

void start_game(Game* game) {
    if (game == NULL) return;


    while (game->is_running) {
        __process_time(game);
        __process_events(game);
        __update(game);
        __render(game);
    }
}

void destroy_game(Game* game) {
    if (game != NULL) {
        SDL_DestroyWindow(game->window);
        SDL_DestroyRenderer(game->renderer);
    }
    
    SDL_Quit();
}


void __process_time(Game* game) {

}

void __process_events(Game* game) {

}

void __update(Game* game) {

}

void __render(Game* game) {
}