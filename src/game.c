#include "game.h"

#define UNUSED(x) (void)x // TODO: remove

static void __init_SDL();
static void __init_window(Game* game);
static void __init_renderer(Game* game);
static void __init_player(Game* game, float x, float y);


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
    __init_player(game, 50.0f, 50.0f);

    game->gevt = init_game_events();
    game->gclock = init_game_clock();


    return game;
}

void start_game(Game* game) {

    while (game->running) {       
        update_game_clock(game->gclock);
        __process_events(game);
        __update(game);
        __render(game);
    }
}

void destroy_game(Game* game) {
    destroy_player(game->player);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);

    destroy_game_clock(game->gclock);
    destroy_game_events(game->gevt);
    

    free(game);
    
    SDL_Quit();
}


void __process_events(Game* game) {
    process_events(game->gevt);
    game->running = !game->gevt->quit;
}

void __update(Game* game) {    
    if (game->gevt->move_left) game->player->position->x -= 0.1f * game->gclock->dt;
    if (game->gevt->move_right) game->player->position->x += 0.1f * game->gclock->dt;
    if (game->gevt->move_up) game->player->position->y -= 0.1f * game->gclock->dt;
    if (game->gevt->move_down) game->player->position->y += 0.1f * game->gclock->dt;
}

void __render(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderClear(game->renderer);
    
    
    SDL_SetRenderDrawColor(game->renderer, 0, 255, 255, 255);
    SDL_Rect rect = { (int)game->player->position->x, (int)game->player->position->y, 25, 25 } ;
    SDL_RenderCopy(game->renderer, game->player->texture, NULL, &rect);
    
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
        SDL_Log("Could not create window: %s\n", SDL_GetError());
        free(game);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void __init_renderer(Game* game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL) {
        SDL_Log("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        free(game);
        exit(EXIT_FAILURE);
    }
}

void __init_player(Game* game, float x, float y) {
    game->player = init_player(game->renderer, x, y);
    if (game->player == NULL) {
        SDL_Quit();
        free(game);
        exit(EXIT_FAILURE);
    }
}