#include "sound.h"


Sound* init_sound() {
    Sound* s = (Sound*)malloc(sizeof(Sound));

    s->music = Mix_LoadMUS("assets/sounds/music/music.mp3");
    if (s->music == NULL) {
        SDL_Log("Did not find music asset: %s", SDL_GetError());
        return NULL;
    }

    if (Mix_PlayMusic(s->music, -1) == -1) {
        SDL_Log("Can't play music: %s", SDL_GetError());
        Mix_FreeMusic(s->music);
        return NULL;
    }

    return s;
}

void destroy_sound(Sound* sound) {
    Mix_FreeMusic(sound->music);
    free(sound);
}