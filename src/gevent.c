#include "gevent.h"


GameEvents* init_game_events() {
    GameEvents* ge = (GameEvents*)malloc(sizeof(GameEvents));
    return ge;
}

void process_events(GameEvents* gevts) {
    gevts->quit = false;
    gevts->move_up = false;
    gevts->move_down = false;
    gevts->move_left = false;
    gevts->move_right = false;


    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        gevts->quit = true;
                        break;
                }
                break;
            case SDL_QUIT:
                gevts->quit = true;
                break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT]) gevts->move_left = true;
    if (state[SDL_SCANCODE_RIGHT]) gevts->move_right = true;
    if (state[SDL_SCANCODE_UP]) gevts->move_up = true;
    if (state[SDL_SCANCODE_DOWN]) gevts->move_down = true;

    if (state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_RIGHT]) {
        gevts->move_left = false;
        gevts->move_right = false;
    }

    if (state[SDL_SCANCODE_UP] && state[SDL_SCANCODE_DOWN]) {
        gevts->move_up = false;
        gevts->move_down = false;
    }
}

void destroy_game_events(GameEvents* gevts) {
    free(gevts);
}