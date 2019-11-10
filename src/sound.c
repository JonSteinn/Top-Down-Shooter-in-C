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
// Free SDL sound chuncks
static const uint32_t FREE_CHUNKS = 1u<<2;

// Path to music file
static const char MUSIC_PATH[] = "assets/sounds/music/music.mp3";
// Path to shoot file
static const char SHOOT_PATH[] = "assets/sounds/effects/shoot.mp3";
// Error message when music file is not found
static const char LOAD_MUSIC_LOG[] = "Did not find music asset: %s";
// Error message when gun sound file is not found
static const char LOAD_SHOOT_LOG[] = "Did not find shoot sound asset: %s";
// Error message when playing music fails
static const char PLAY_MUSIC_LOG[] = "Can't play music: %s";

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
 *  __load_chunks
 *
 * Purpose:
 *  Load the sound effects files.
 *
 * Parameters:
 *  - sound:
 *      A Sound object that stores the sound effect data.
 *
 * Returns:
 *  true if successful, false otherwise.
 */
static bool __load_chunks(Sound* sound);

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
 *      FREE_CHUNKS
 *
 * Returns:
 *  Nothing.
 */
static void __destroy(Sound* sound, uint32_t mask);

/**
 * If the function fails midway, make sure any
 * previously allocated resources are released.
 */
Sound* init_sound(void) {
    // Allocate memory
    Sound* s = (Sound*)malloc(sizeof(Sound));

    // Load music
    if (!__load_music(s)) return NULL;

    // Load sound effects
    if (!__load_chunks(s)) return NULL;

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
    sound->music = Mix_LoadMUS(MUSIC_PATH);

    // If fails
    if (sound->music == NULL) {
        SDL_Log(LOAD_MUSIC_LOG, SDL_GetError());
        __destroy(sound, FREE_MEMORY);
        return false;
    }

    return true;
}

/**
 * Release memory of sound and music [no pun intended] if
 * we fail to load chunks.
 */
static bool __load_chunks(Sound* sound) {
    sound->shoot = Mix_LoadWAV(SHOOT_PATH);

    // If fails
    if (sound->shoot == NULL) {
        SDL_Log(LOAD_SHOOT_LOG, SDL_GetError());

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
        SDL_Log(PLAY_MUSIC_LOG, SDL_GetError());
        __destroy(sound, FREE_MUSIC | FREE_MEMORY | FREE_CHUNKS);
        return false;
    }

    return true;
}

/**
 * Check each resources against mask before releasing.
 */
static void __destroy(Sound* sound, uint32_t mask) {
    if (mask & FREE_MUSIC) Mix_FreeMusic(sound->music);
    if (mask & FREE_CHUNKS) Mix_FreeChunk(sound->shoot);
    if (mask & FREE_MEMORY) free(sound);
}