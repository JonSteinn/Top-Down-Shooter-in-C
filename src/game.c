#include "game.h"

#define UNUSED(x) (void)x // TODO: remove

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
    game->running = true;
    
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
    if (game->gevt->quit) game->running = false;
}

float x = 50, y = 50; // TODO: Remove
void __update(Game* game) {
    if (game->gevt->move_left) x -= 0.15 * game->gclock->dt;
    if (game->gevt->move_right) x += 0.15 * game->gclock->dt;
    if (game->gevt->move_up) y -= 0.15 * game->gclock->dt;
    if (game->gevt->move_down) y += 0.15 * game->gclock->dt;
}

void __render(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderClear(game->renderer);
    SDL_SetRenderDrawColor(game->renderer, 0, 255, 255, 255);
    SDL_Rect rect = { (int)x, (int)y, 200, 200 };
    SDL_RenderFillRect(game->renderer, &rect);
    SDL_RenderPresent(game->renderer);
}