#ifndef pLe9jzg2Qv_SOUND_H
#define pLe9jzg2Qv_SOUND_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
    Mix_Music* music;
} Sound;

Sound* init_sound();
void destroy_sound(Sound* sound);


#endif