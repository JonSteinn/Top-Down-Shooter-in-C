#include "game.h"

#define UNUSED(x) (void)x // TODO: remove

static void __init_SDL();
static void __init_window(Game* game);
static void __init_renderer(Game* game);

static void __process_events(Game* game);
static void __update(Game* game);
static void __render(Game* game);

Game* init_game(int32_t argc, char** args) {
    UNUSED(argc);UNUSED(args); // TODO: remove
    __init_SDL();
    Game* game = (Game*)malloc(sizeof(Game));
    game->running = true;
    __init_window(game);
    __init_renderer(game);
    game->gclock = init_game_clock();
    game->gevt = init_game_events();
    return game;
}

void start_game(Game* game) {
    if (game == NULL) return;

    while (game->running) {       
        update_game_clock(game->gclock);
        __process_events(game);
        __update(game);
        __render(game);
    }
}

void destroy_game(Game* game) {
    if (game != NULL) {
        SDL_DestroyWindow(game->window);
        SDL_DestroyRenderer(game->renderer);

        destroy_game_clock(game->gclock);
    }
    
    SDL_Quit();
}


void __process_events(Game* game) {
    process_events(game->gevt);
    game->running = !game->gevt->quit;
}

void __update(Game* game) {
    UNUSED(game);
}

void __render(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderClear(game->renderer);
    
    
    
    
    SDL_RenderPresent(game->renderer);
}

void __init_SDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void __init_window(Game* game) {
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
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void __init_renderer(Game* game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        free(game);
        exit(EXIT_FAILURE);
    }
}