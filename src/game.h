#ifndef dR4wsjUCND_GAME_H
#define dR4wsjUCND_GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
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
 * The main game object that keeps hold of all resources.
 * 
 *  width
 *      The window's width 
 *  height
 *      The window's height
 *  window
 *      The os window
 *  renderer
 *      The SDL renderer to which we draw
 *  running
 *      Should the game loop run?
 *  gclock
 *      The game's clock
 *  gevts
 *      The game's events
 *  player
 *      The game's player object
 *  floor
 *      The game's background tile
 *  sound
 *      The object that handles all in-game audio
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
 * Initialize SDL and all game resources.
 */
Game* init_game(int32_t argc, char** args);

/**
 * Start game loop.
 */
void start_game(Game* game);

/**
 * Free all game resources.
 */
void destroy_game(Game* game);

#endif