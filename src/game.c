#include "game.h"

static const char TITLE[] = "Top dow shooter in C";
static const int32_t DEFAULT_WIDTH = 800;
static const int32_t DEFAULT_HEIGHT = 600;
static const int32_t DEFAULT_ENEMY_COUNT = 10;

static void __init_SDL();
static void __get_screen_resolution(int* w, int* h);
static void __parse_arguments(Game* game, int32_t argc, char** args, int32_t w, int32_t h);
static void __init_window(Game* game, int32_t w, int32_t h);
static void __init_renderer(Game* game);
static void __init_sound(Game* game);
static void __init_player(Game* game, float x, float y);
static void __init_enemies(Game* game, int32_t count);
static void __init_floor(Game* game);
static void __process_events(Game* game);
static void __update(Game* game);
static void __render(Game* game);

Game* init_game(int32_t argc, char** args) {
    srand(time(NULL));

    __init_SDL();

    int32_t w, h;
    __get_screen_resolution(&w, &h);

    Game* game = (Game*)malloc(sizeof(Game));
    game->running = true;
    game->width = DEFAULT_WIDTH;
    game->height = DEFAULT_HEIGHT;

    __parse_arguments(game, argc, args, w, h);

    __init_window(game, w, h);
    __init_renderer(game);
    __init_sound(game);
    __init_player(game, game->width / 2.0f, game->height / 2.0f);
    __init_enemies(game, DEFAULT_ENEMY_COUNT);
    __init_floor(game);


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
    destroy_floor(game->floor);
    destroy_enemies(game->enemies);
    destroy_player(game->player);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    destroy_game_clock(game->gclock);
    destroy_game_events(game->gevts);
    destroy_sound(game->sound);
    Mix_CloseAudio();
    free(game);
    SDL_Quit();
}


static void __get_screen_resolution(int* w, int* h) {
    SDL_DisplayMode DM;
    if (SDL_GetDesktopDisplayMode(0, &DM) < 0) {
        SDL_Log("Getting screen resolution failed: %s", SDL_GetError());
        Mix_CloseAudio();
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
    update_player(game->player, game->gevts, game->gclock->dt, game->width, game->height);
    update_enemies(game->enemies, game->gclock->dt, &game->player->position);
}

void __render(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderClear(game->renderer);

    draw_floor(game->renderer, game->floor, game->width, game->height);
    draw_enemies(game->renderer, game->enemies);
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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1<<12) == -1) {
        SDL_Log("Unable to initialize SDL audio: %s", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void __init_window(Game* game, int32_t w, int32_t h) {
    bool full_screen = game->width == w || game->height == h;
    Uint32 flags = SDL_WINDOW_OPENGL;
    if (full_screen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    game->window = SDL_CreateWindow(
        TITLE,                      // window title
        SDL_WINDOWPOS_UNDEFINED,    // initial x position
        SDL_WINDOWPOS_UNDEFINED,    // initial y position
        game->width,                // width, in pixels
        game->height,               // height, in pixels
        flags                       // flags
    );
    if (game->window == NULL) {
        SDL_Log("Could not create window: %s\n", SDL_GetError());
        free(game);
        Mix_CloseAudio();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void __init_renderer(Game* game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL) {
        SDL_Log("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        Mix_CloseAudio();
        SDL_Quit();
        free(game);
        exit(EXIT_FAILURE);
    }
}

void __init_sound(Game* game) {
    game->sound = init_sound();
    if (game->sound == NULL) {
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        Mix_CloseAudio();
        SDL_Quit();
        free(game);
        exit(EXIT_FAILURE);
    }
}

void __init_player(Game* game, float x, float y) {
    game->player = init_player(game->renderer, x, y);
    if (game->player == NULL) {
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        Mix_CloseAudio();
        SDL_Quit();
        free(game);
        exit(EXIT_FAILURE);
    }
}

void __init_enemies(Game* game, int32_t count) {
    game->enemies = init_enemies(game->renderer, count, game->width, game->height);
    if (game->enemies == NULL) {
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        Mix_CloseAudio();
        SDL_Quit();
        destroy_player(game->player);
        free(game);
        exit(EXIT_FAILURE);
    }
}

void __init_floor(Game* game) {
    game->floor = init_floor(game->renderer);
    if (game->floor == NULL) {
        destroy_player(game->player);
        destroy_enemies(game->enemies);
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        Mix_CloseAudio();
        SDL_Quit();
        free(game);
        exit(EXIT_FAILURE);
    }
}
