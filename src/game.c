#include "game.h"

static void __init_SDL();
static void __get_screen_resolution(int* w, int* h);
static void __parse_arguments(Game* game, int32_t argc, char** args, int32_t w, int32_t h);
static void __init_window(Game* game, int32_t w, int32_t h);
static void __init_renderer(Game* game);
static void __init_player(Game* game, float x, float y);
static void __process_events(Game* game);
static void __update(Game* game);
static void __render(Game* game);

Game* init_game(int32_t argc, char** args) {

    __init_SDL();

    int32_t w, h;
    __get_screen_resolution(&w, &h);

    Game* game = (Game*)malloc(sizeof(Game));
    game->running = true;
    game->width = 800;
    game->height = 600;

    __parse_arguments(game, argc, args, w, h);

    __init_window(game, w, h);
    __init_renderer(game);
    __init_player(game, game->width / 2.0f, game->height / 2.0f);

    game->gevts = init_game_events();
    game->gclock = init_game_clock();


    return game;
}

void start_game(Game* game) {
    // GAME LOOP
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
    destroy_game_events(game->gevts);
    

    free(game);
    
    SDL_Quit();
}


static void __get_screen_resolution(int* w, int* h) {
    SDL_DisplayMode DM;
    if (SDL_GetDesktopDisplayMode(0, &DM) < 0) {
        SDL_Log("Getting screen resolution failed: %s", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    *w = DM.w;
    *h = DM.h;
}


void __process_events(Game* game) {
    process_events(game->gevts);
    game->running = !game->gevts->quit;
}


void __update(Game* game) {
    update_player(game->player, game->gevts, game->gclock->dt);
}

void __render(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderClear(game->renderer);
    
    draw_player(game->renderer, game->player);
    
    SDL_RenderPresent(game->renderer);
}

void __parse_arguments(Game* game, int32_t argc, char** args, int32_t w, int32_t h) {

    for (int32_t i = 1; i <= argc; i++) {
        if (args[i] && args[i][0] && args[i][1] && args[i][2] 
            && (args[i][0] == 'w' || args[i][0] == 'h') && args[i][1] == '=') {

                int32_t v = string_to_int(args[i]+2);
                if (args[i][0] == 'w') game->width = v < 400 ? 400 : v;
                if (args[i][0] == 'h') game->height = v < 400 ? 400 : v;
        }
    }

    if (game->width > 0.9 * w || game->height > 0.9 * h) {
        game->width = w;
        game->height = h;
    }
}

void __init_SDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void __init_window(Game* game, int32_t w, int32_t h) {
    bool full_screen = game->width == w || game->height == h;
    Uint32 flags = SDL_WINDOW_OPENGL;
    if (full_screen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    game->window = SDL_CreateWindow(
        "Top dow shooter in C",     // window title
        SDL_WINDOWPOS_UNDEFINED,    // initial x position
        SDL_WINDOWPOS_UNDEFINED,    // initial y position
        game->width,                // width, in pixels
        game->height,               // height, in pixels
        flags                       // flags
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