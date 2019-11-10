#ifndef pLe9jzg2Qv_SOUND_H
#define pLe9jzg2Qv_SOUND_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/**
 * Struct:
 *  Sound
 *
 * Purpose:
 *  Holds on to any sound resources needed by the game.
 *
 * Fields:
 *  - music:
 *      This is an opaque data type used for Music data.
 */
typedef struct {
    Mix_Music* music;
} Sound;

/**
 * Function:
 *  init_sound
 *
 * Purpose:
 *  Create and initialize a Sound object.
 *
 * Parameters:
 *  None.
 *
 * Returns:
 *  Sound object if successful, NULL otherwise.
 */
Sound* init_sound(void);

/**
 * Function:
 *  destroy_sound
 *
 * Purpose:
 *  Release all resources of a Sound object.
 *
 * Parameters:
 *  - sound:
 *      The Sound object to destroy.
 *
 * Returns:
 *  Nothing.
 */
void destroy_sound(Sound* sound);

#endif