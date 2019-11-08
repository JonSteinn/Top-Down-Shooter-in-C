#include "sound.h"

/*************
 * Bit masks *
 *************/
// Release all resource
static const uint32_t FREE_ALL = 0xFFFFFFFFu;
// Release SDL music data
static const uint32_t FREE_MUSIC = 1u<<0;
// Free Sound object
static const uint32_t FREE_MEMORY = 1u<<1;

// Path to music file
static const char music_path[] = "assets/sounds/music/music.mp3";
// Error message when music file is not found
static const char load_music_log[] = "Did not find music asset: %s";
// Error message when playing music fails
static const char play_music_log[] = "Can't play music: %s";

/**
 * Function:
 *  __load_music
 * 
 * Purpose:
 *  Load the music file.
 * 
 * Parameters:
 *  - sound:
 *      A Sound object that stores the music data.
 * 
 * Returns:
 *  true if successful, false otherwise.
 */
static bool __load_music(Sound* sound);

/**
 * Function:
 *  __play_music
 * 
 * Purpose:
 *  Starts playing the music in an infinite loop.
 * 
 * Parameters:
 *  - sound:
 *      A Sound object that stores the music data.
 * 
 * Returns:
 *  true if successful, false otherwise.
 */
static bool __play_music(Sound* sound);

/**
 * Function:
 *  __destroy
 * 
 * Purpose:
 *  Release resources of the Sound object.
 * 
 * Parameters:
 *  - sound:
 *      The Sound object to destroy
 *  - mask:
 *      A mask to choose which resources are destroyed.
 *      FREE_ALL
 *      FREE_MUSIC
 *      FREE_MEMORY
 * 
 * Returns:
 *  Nothing.
 */
static void __destroy(Sound* sound, uint32_t mask);

/**
 * If the function fails midway, make sure any
 * previously allocated resources are released.
 */
Sound* init_sound() {
    // Allocate memory
    Sound* s = (Sound*)malloc(sizeof(Sound));

    // Load music
    if (!__load_music(s)) return NULL;

    // Play music
    if (!__play_music(s)) return NULL;

    return s;
}

/**
 * First free all SDL related resources before
 * releasing the memory of the object.
 */
void destroy_sound(Sound* sound) {
    __destroy(sound, FREE_ALL);
}

/**
 * Releases memory of sound if fails to load music.
 */
static bool __load_music(Sound* sound) {
    sound->music = Mix_LoadMUS(music_path);

    // If fails
    if (sound->music == NULL) {
        SDL_Log(load_music_log, SDL_GetError());
        __destroy(sound, FREE_MEMORY);
        return false;
    }

    return true;
}

/**
 * Releases resources of sound and loaded music
 * if fails to play music.
 */
static bool __play_music(Sound* sound) {
    // If fails
    if (Mix_PlayMusic(sound->music, -1) == -1) {
        SDL_Log(play_music_log, SDL_GetError());
        __destroy(sound, FREE_MUSIC | FREE_MEMORY);
        return false;
    }

    return true;
}

/**
 * Check each resources against mask before releasing.
 */
static void __destroy(Sound* sound, uint32_t mask) {
    if (mask & FREE_MUSIC) Mix_FreeMusic(sound->music);
    if (mask & FREE_MEMORY) free(sound);
}