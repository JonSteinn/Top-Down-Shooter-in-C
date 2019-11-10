#include "game.h"

/*************
 * Bit masks *
 *************/
// Release all resources
static const uint32_t FREE_ALL = 0xFFFFFFFFu;
// Release SDL resources
static const uint32_t FREE_SDL = 1u<<0;
// Release SDL_mixer resources
static const uint32_t FREE_SDL_AUDIO = 1u<<1;
// Free Game object
static const uint32_t FREE_MEMORY = 1u<<2;
// Destroy Sound object
static const uint32_t FREE_SOUND = 1u<<3;
// Destroy  object
static const uint32_t FREE_EVENTS = 1u<<4;
// Destroy  object
static const uint32_t FREE_CLOCK = 1u<<5;
// Free window resources
static const uint32_t FREE_WINDOW = 1u<<6;
// Free renderer resources
static const uint32_t FREE_RENDERER = 1u<<7;
// Destroy Player object
static const uint32_t FREE_PLAYER = 1u<<8;
// Destroy Enemies object
static const uint32_t FREE_ENEMIES = 1u<<9;
// Destroy Floor object
static const uint32_t FREE_FLOOR = 1u<<10;

// Error message when initializing SDL fails
static const char INIT_SDL_LOG[] = "Unable to initialize SDL: %s";
// Error message when initializing SDL_mixer fails
static const char OPEN_AUDIO_LOG[] = "Unable to initialize SDL audio: %s";
// Error message when we fail to get screen resolution
static const char DISPLAY_MODE_LOG[] = "Getting screen resolution failed: %s";
// Error message when we fail to create window
static const char CREATE_WIN_LOG[] = "Could not create window: %s\n";
// Error message when we fail to create renderer
static const char CREATE_RENDERER_LOG[] = "Could not create renderer: %s\n";
// Game's title
static const char TITLE[] = "Top dow shooter in C";
// Default width if no or invalid argument
static const int32_t DEFAULT_WIDTH = 800;
// Default height if no or invalid argument
static const int32_t DEFAULT_HEIGHT = 600;
// The smallest allowed window size (both directions)
static const int32_t MIN_WINDOW_DIM = 400;
// Default enemy count if no or invalid argument
static const int32_t DEFAULT_ENEMY_COUNT = 10;
// The smallest possible amount of enemies
static const int32_t MIN_ENEMY_COUNT = 1;
// The largest possible amount of enemies
static const int32_t MAX_ENEMY_COUNT = 10000;
// Number of audio channels (2 = stereo)
static const int32_t AUDIO_CHANNELS = 2;
// Bytes used per output sample (audio)
static const int32_t AUDIO_CHUNK_SIZE = 1<<12;
// Maximum ratio of resolution before switching to full screen
static const float MAX_DIM_RATIO = 0.9f;

/**
 * Function:
 *  __init_SDL
 *
 * Purpose:
 *  Initialize SDL2 and SDL2_mixer.
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  Nothing.
 */
static void __init_SDL(void);

/**
 * Function:
 *  __destroy
 *
 * Purpose:
 *  Release resources of the Game object.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *  - mask:
 *      A mask to choose which resources are destroyed.
 *      FREE_ALL
 *      FREE_SDL
 *      FREE_SDL_AUDIO
 *      FREE_MEMORY
 *      FREE_SOUND
 *      FREE_EVENTS
 *      FREE_CLOCK
 *      FREE_WINDOW
 *      FREE_RENDERER
 *      FREE_PLAYER
 *      FREE_ENEMIES
 *      FREE_FLOOR
 *
 * Returns:
 *  Nothing.
 */
static void __destroy(Game* game, uint32_t mask);

/**
 * Function:
 *  __get_screen_resolution
 *
 * Purpose:
 *  Get the maximum window width and height the running machine supports.
 *
 * Parameters:
 *  - w:
 *      An integer to set width.
 *  - h:
 *      An integer to set height.
 *
 * Returns:
 *  Nothing.
 */
static void __get_screen_resolution(int* w, int* h);

/**
 * Function:
 *  __alloc_and_set_game
 *
 * Purpose:
 *  Allocate memory for Game object and initialize
 *  some of its values.
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  A Game object.
 */
static Game* __alloc_and_set_game(void);

/**
 * Function:
 *  __parse_arguments
 *
 * Purpose:
 *  Parse and validate command line arguments.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *  - argc:
 *      The number of arguments.
 *  - argv:
 *      A list of arguments.
 *  - w:
 *      The screen resolution's width.
 *  - h:
 *      The screen resolution's height.
 *  - z:
 *      An integer to store number of enemies.
 *
 * Returns:
 *  Nothing.
 */
static void __parse_arguments(Game* game, int32_t argc, char** argv, int32_t w, int32_t h, int32_t* z);

/**
 * Function:
 *  __init_window
 *
 * Purpose:
 *  Create and initialize a window.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *  - w:
 *      The screen resolution's width.
 *  - h:
 *      The screen resolution's height.
 *
 * Returns:
 *  Nothing.
 */
static void __init_window(Game* game, int32_t w, int32_t h);

/**
 * Function:
 *  __init_renderer
 *
 * Purpose:
 *  Create a 2d rendering context for the window.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *
 * Returns:
 *  Nothing.
 */
static void __init_renderer(Game* game);

/**
 * Function:
 *  __init_sound
 *
 * Purpose:
 *  Initialize our sound subsystem.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *
 * Returns:
 *  Nothing.
 */
static void __init_sound(Game* game);

/**
 * Function:
 *  __init_player
 *
 * Purpose:
 *  Initialize the player within the game.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *  - x:
 *      The player's starting position's x coordinate.
 *  - y:
 *      The player's starting position's y coordinate.
 *
 * Returns:
 *  Nothing.
 */
static void __init_player(Game* game, float x, float y);

/**
 * Function:
 *  __init_enemies
 *
 * Purpose:
 *  Initialize the enemies within the game.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *  - count:
 *      The number of enemies the game should contain.
 *
 * Returns:
 *  Nothing.
 */
static void __init_enemies(Game* game, int32_t count);

/**
 * Function:
 *  __init_floor
 *
 * Purpose:
 *  Initialize the floor within the game.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *
 * Returns:
 *  Nothing.
 */
static void __init_floor(Game* game);

/**
 * Function:
 *  __process_events
 *
 * Purpose:
 *  Map SDL events to our game specific events and
 *  check if game loop should continue.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *
 * Returns:
 *  Nothing.
 */
static void __process_events(Game* game);

/**
 * Function:
 *  __update
 *
 * Purpose:
 *  Update everything that should be updated this frame.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *
 * Returns:
 *  Nothing.
 */
static void __update(Game* game);

/**
 * Function:
 *  __render
 *
 * Purpose:
 *  Render everything that should be drawn this frame.
 *
 * Parameters:
 *  - game:
 *      The Game object.
 *
 * Returns:
 *  Nothing.
 */
static void __render(Game* game);

/**
 * At any point, if anything fails, we clean previously
 * allocated resources and exit with 1.
 */
Game* init_game(int32_t argc, char** argv) {
    // Seed the random generator based on current time
    srand(time(NULL));

    __init_SDL();

    int32_t w, h, z = DEFAULT_ENEMY_COUNT;
    __get_screen_resolution(&w, &h);

    Game* game = __alloc_and_set_game();

    __parse_arguments(game, argc, argv, w, h, &z);
    __init_window(game, w, h);
    __init_renderer(game);
    __init_sound(game);
    __init_player(game, game->width / 2.0f, game->height / 2.0f);
    __init_enemies(game, z);
    __init_floor(game);

    game->gevts = init_game_events();
    game->gclock = init_game_clock();

    return game;
}

/**
 * 1. Update delta time for scalinng
 * 2. Map SDL2 events to our game specific events
 * 3. Update all game objects
 * 4. Render all game objects
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
 * Release all resources within the game.
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
    if (FREE_MEMORY & mask) free(game);
    if (FREE_SDL_AUDIO & mask) Mix_CloseAudio();
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
 * Allocate memory for a Game object and set some of its values.
 */
static Game* __alloc_and_set_game(void) {
    Game* game = (Game*)malloc(sizeof(Game));
    game->running = true;
    game->width = DEFAULT_WIDTH;
    game->height = DEFAULT_HEIGHT;
    return game;
}

/**
 * Parse flags -w, -h and -z with getopt. All are expected to have values.
 * If invalid (either non-numeric or too small/large), then we use default
 * values. All values have been set prior to this so if arguments are missing,
 * they are still initialized to some value.
 */
static void __parse_arguments(Game* game, int32_t argc, char** argv, int32_t w, int32_t h, int32_t* z) {
    int32_t opt, v;
    while ((opt = getopt(argc, argv, "w:h:z:")) != -1) {
        switch (opt) {
            case 'w':
                v = string_to_int(optarg);
                game->width = v < MIN_WINDOW_DIM ? MIN_WINDOW_DIM : v;
                break;
            case 'h':
                v = string_to_int(optarg);
                game->height = v < MIN_WINDOW_DIM ? MIN_WINDOW_DIM : v;
                break;
            case 'z':
                v = string_to_int(optarg);
                if (MIN_ENEMY_COUNT <= v && v <= MAX_ENEMY_COUNT) *z = v;
                break;
            default:
                break;
            }
    }

    // If we reach a certain size, close to full screen, we set the window to full screen.
    if (game->width > MAX_DIM_RATIO * w || game->height > MAX_DIM_RATIO * h) {
        game->width = w;
        game->height = h;
    }
}

/**
 * If either the game width or height is at maximum for the screen,
 * we opt for full screen mode. If we fail to create the window, we
 * terminate right here but first release any previously allocated
 * resources.
 */
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
        SDL_Log(CREATE_WIN_LOG, SDL_GetError());
        __destroy(game, FREE_MEMORY | FREE_SDL | FREE_SDL_AUDIO);
        exit(EXIT_FAILURE);
    }
}

/**
 * If we fail to create renderer we terminate here but first release
 * any previously allocated resources.
 */
static void __init_renderer(Game* game) {
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (game->renderer == NULL) {
        SDL_Log(CREATE_RENDERER_LOG, SDL_GetError());
        __destroy(game, FREE_MEMORY | FREE_SDL | FREE_SDL_AUDIO | FREE_WINDOW);
        exit(EXIT_FAILURE);
    }
}

/**
 * If we fail to create sound we terminate here but first release
 * any previously allocated resources.
 */
static void __init_sound(Game* game) {
    game->sound = init_sound();
    if (game->sound == NULL) {
        __destroy(game, FREE_MEMORY | FREE_SDL | FREE_SDL_AUDIO | FREE_WINDOW | FREE_RENDERER);
        exit(EXIT_FAILURE);
    }
}

/**
 * If we fail to create player we terminate here but first release
 * any previously allocated resources.
 */
static void __init_player(Game* game, float x, float y) {
    game->player = init_player(game->renderer, x, y);
    if (game->player == NULL) {
        __destroy(game, FREE_MEMORY | FREE_SDL | FREE_SDL_AUDIO |
            FREE_WINDOW | FREE_RENDERER | FREE_SOUND);
        exit(EXIT_FAILURE);
    }
}

/**
 * If we fail to create enemies we terminate here but first release
 * any previously allocated resources.
 */
static void __init_enemies(Game* game, int32_t count) {
    game->enemies = init_enemies(game->renderer, count, game->width, game->height);
    if (game->enemies == NULL) {
        __destroy(game, FREE_MEMORY | FREE_SDL | FREE_SDL_AUDIO |
            FREE_WINDOW | FREE_RENDERER | FREE_SOUND | FREE_PLAYER);
        exit(EXIT_FAILURE);
    }
}

/**
 * If we fail to create floor we terminate here but first release
 * any previously allocated resources.
 */
static void __init_floor(Game* game) {
    game->floor = init_floor(game->renderer);
    if (game->floor == NULL) {
        __destroy(game, FREE_MEMORY | FREE_SDL | FREE_SDL_AUDIO | FREE_WINDOW |
            FREE_RENDERER | FREE_SOUND | FREE_PLAYER | FREE_ENEMIES);
        exit(EXIT_FAILURE);
    }
}

/**
 * After mapping events, we update weather to keep game loop going.
 */
static void __process_events(Game* game) {
    process_events(game->gevts);
    game->running = !game->gevts->quit;
}

/**
 * Calls update on all update-able game objects.
 */
static void __update(Game* game) {
    update_player(game->player, game->gevts, game->gclock->dt, game->width, game->height);
    update_enemies(game->enemies, game->gclock->dt, &game->player->position);
}

/**
 * We start by clearing the screen with black, then render all objects of the game.
 */
static void __render(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderClear(game->renderer);

    draw_floor(game->renderer, game->floor, game->width, game->height);
    draw_enemies(game->renderer, game->enemies, game->width, game->height);
    draw_player(game->renderer, game->player);

    SDL_RenderPresent(game->renderer);
}