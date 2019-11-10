#include "game.h"

/*************
 * Bit masks *
 *************/
// Release all resources
static const uint32_t FREE_ALL = 0xFFFFFFFFu;
static const uint32_t FREE_SDL = 1u<<0;
static const uint32_t FREE_SDL_AUDIO = 1u<<1;
static const uint32_t FREE_MEMORY = 1u<<2;
static const uint32_t FREE_SOUND = 1u<<3;
static const uint32_t FREE_EVENTS = 1u<<4;
static const uint32_t FREE_CLOCK = 1u<<5;
static const uint32_t FREE_WINDOW = 1u<<6;
static const uint32_t FREE_RENDERER = 1u<<7;
static const uint32_t FREE_PLAYER = 1u<<8;
static const uint32_t FREE_ENEMIES = 1u<<9;
static const uint32_t FREE_FLOOR = 1u<<10;

static const char INIT_SDL_LOG[] = "Unable to initialize SDL: %s";
static const char OPEN_AUDIO_LOG[] = "Unable to initialize SDL audio: %s";
static const char DISPLAY_MODE_LOG[] = "Getting screen resolution failed: %s";
static const char TITLE[] = "Top dow shooter in C";
static const int32_t DEFAULT_WIDTH = 800;
static const int32_t DEFAULT_HEIGHT = 600;
static const int32_t DEFAULT_ENEMY_COUNT = 10;
static const int32_t AUDIO_CHANNELS = 2;
static const int32_t AUDIO_CHUNK_SIZE = 1<<12;

static void __init_SDL(void);
static void __destroy(Game* game, uint32_t mask);
static void __get_screen_resolution(int* w, int* h);
static Game* __alloc_and_set_game(void);
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












/**
 *
 */
Game* init_game(int32_t argc, char** argv) {
    // Seed the random generator based on current time
    srand(time(NULL));

    __init_SDL();

    int32_t w, h;
    __get_screen_resolution(&w, &h);

    Game* game = __alloc_and_set_game();

    __parse_arguments(game, argc, argv, w, h);
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

/**
 *
 */
void start_game(Game* game) {
    // GAME LOOP
    while (game->running) {
        update_game_clock(game->gclock);
        __process_events(game);
        __update(game);
        __render(game);
    }
}

/**
 *
 */
void destroy_game(Game* game) {
    __destroy(game, FREE_ALL);
}






/**
 * Initialize SDL2 and SDL2_mixer. If the first fails, we won't
 * bother with the second and if the latter fails, we clean the
 * resources of the first one. In the case where either fail,
 * we terminate the program.
 */
static void __init_SDL(void) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log(INIT_SDL_LOG, SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialize SDL2_mixer
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNK_SIZE) == -1) {
        SDL_Log(OPEN_AUDIO_LOG, SDL_GetError());
        __destroy(NULL, FREE_SDL);
        exit(EXIT_FAILURE);
    }
}

/**
 * Check each resources against mask before releasing.
 */
static void __destroy(Game* game, uint32_t mask) {
    if (FREE_FLOOR & mask) destroy_floor(game->floor);
    if (FREE_ENEMIES & mask) destroy_enemies(game->enemies);
    if (FREE_PLAYER & mask) destroy_player(game->player);
    if (FREE_RENDERER & mask) SDL_DestroyRenderer(game->renderer);
    if (FREE_WINDOW & mask) SDL_DestroyWindow(game->window);
    if (FREE_CLOCK & mask) destroy_game_clock(game->gclock);
    if (FREE_EVENTS & mask) destroy_game_events(game->gevts);
    if (FREE_SOUND & mask) destroy_sound(game->sound);
    if (FREE_SDL_AUDIO & mask) Mix_CloseAudio();
    if (FREE_MEMORY & mask) free(game);
    if (FREE_SDL & mask) SDL_Quit();
}

/**
 * If getting display fails, we terminate here.
 */
static void __get_screen_resolution(int* w, int* h) {
    SDL_DisplayMode DM;
    if (SDL_GetDesktopDisplayMode(0, &DM) < 0) {
        SDL_Log(DISPLAY_MODE_LOG, SDL_GetError());
        __destroy(NULL, FREE_SDL_AUDIO | FREE_SDL);
        exit(EXIT_FAILURE);
    }
    *w = DM.w;
    *h = DM.h;
}

/**
 *
 */
static Game* __alloc_and_set_game(void) {
    Game* game = (Game*)malloc(sizeof(Game));
    game->running = true;
    game->width = DEFAULT_WIDTH;
    game->height = DEFAULT_HEIGHT;
    return game;
}


static void __parse_arguments(Game* game, int32_t argc, char** argv, int32_t w, int32_t h) {
    int32_t opt, v;
    while ((opt = getopt(argc, argv, "w:h:")) != -1) {
        switch (opt) {
            case 'w':
                v = string_to_int(optarg);
                game->width = v < 400 ? 400 : v;
                break;
            case 'h':
                v = string_to_int(optarg);
                game->height = v < 400 ? 400 : v;
                break;
            default:
                break;
            }
    }

    if (game->width > 0.9 * w || game->height > 0.9 * h) {
        game->width = w;
        game->height = h;
    }
}

















static void __process_events(Game* game) {
    process_events(game->gevts);
    game->running = !game->gevts->quit;
}

static void __update(Game* game) {
    update_player(game->player, game->gevts, game->gclock->dt, game->width, game->height);
    update_enemies(game->enemies, game->gclock->dt, &game->player->position);
}

static void __render(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderClear(game->renderer);

    draw_floor(game->renderer, game->floor, game->width, game->height);
    draw_enemies(game->renderer, game->enemies);
    draw_player(game->renderer, game->player);

    SDL_RenderPresent(game->renderer);
}





static void __init_window(Game* game, int32_t w, int32_t h) {
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

static void __init_renderer(Game* game) {
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

static void __init_sound(Game* game) {
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

static void __init_player(Game* game, float x, float y) {
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

static void __init_enemies(Game* game, int32_t count) {
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

static void __init_floor(Game* game) {
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
