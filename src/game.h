#ifndef dR4wsjUCND_GAME_H
#define dR4wsjUCND_GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#include <SDL2/SDL.h>

#include "gclock.h"
#include "gevent.h"
#include "player.h"
#include "gmath.h"
#include "utils.h"
#include "floor.h"
#include "sound.h"
#include "enemies.h"

/**
 * Struct:
 *  Game
 *
 * Purpose:
 *  Holds on to all of the games resources.
 *
 * Fields:
 *  - width:
 *      The window's width.
 *  - height:
 *      The window's height
 *  - window:
 *      The type used to identify a window.
 *  - renderer:
 *      A structure that contains a rendering state.
 *  - running:
 *      Should the game loop keep running?
 *  - gclock:
 *      Keeps track of time between loops.
 *  - gevts:
 *      Maps SDL events to game specific events.
 *  - player:
 *      Handles everything player related.
 *  - enemies:
 *      Handles everything enemy related.
 *  - floor:
 *      To draw the background.
 *  - sound:
 *      The game's sound subsystem, which handles playing sounds.
 */
typedef struct {
    int32_t         width;
    int32_t         height;
    SDL_Window*     window;
    SDL_Renderer*   renderer;
    bool            running;
    GameClock*      gclock;
    GameEvents*     gevts;
    Player*         player;
    Enemies*        enemies;
    Floor*          floor;
    Sound*          sound;
} Game;

/**
 * Function:
 *  init_game
 *
 * Purpose:
 *  Allocate memory for game related resources and initialize them.
 *
 * Parameters:
 * - argc:
 *      The number of arguments.
 * - argv:
 *      The list of arguments.
 *
 * Returns:
 *  An initialized Game object.
 */
Game* init_game(int32_t argc, char** argv);

/**
 * Function:
 *  start_game
 *
 * Purpose:
 *  Start the game loop.
 *
 * Parameters:
 *  A Game object
 *
 * Returns:
 *  Nothing.
 */
void start_game(Game* game);

/**
 * Function:
 *  destroy_game
 *
 * Purpose:
 *  Clean up any resources used by the game.
 *
 * Parameters:
 *  - game:
 *      The Game object to destroy.
 *
 * Returns:
 *  Nothing.
 */
void destroy_game(Game* game);

#endif