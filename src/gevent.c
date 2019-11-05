#include "gevent.h"

static void __set_to_default(GameEvents* gevts);
static void __poll_events(GameEvents* gevts);
static void __keyboard_state(GameEvents* gevts);

GameEvents* init_game_events() {
    GameEvents* gevts = (GameEvents*)malloc(sizeof(GameEvents));
    __set_to_default(gevts);
    return gevts;
}

void process_events(GameEvents* gevts) {
    __set_to_default(gevts);
    __poll_events(gevts);
    __keyboard_state(gevts);

    gevts->shoot = SDL_GetMouseState(&gevts->mouseX, &gevts->mouseY) & SDL_BUTTON(1);
}

void destroy_game_events(GameEvents* gevts) {
    free(gevts);
}

void __set_to_default(GameEvents* gevts) {
    gevts->quit         = false;
    gevts->move_up      = false;
    gevts->move_down    = false;
    gevts->move_left    = false;
    gevts->move_right   = false;
    gevts->shoot        = false;
}

void __poll_events(GameEvents* gevts) {
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
}

void __keyboard_state(GameEvents* gevts) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D]) {
        gevts->move_left = true;
    } else if (!state[SDL_SCANCODE_A] && state[SDL_SCANCODE_D]) {
        gevts->move_right = true;
    }

    if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]) {
        gevts->move_up = true;
    } else if (!state[SDL_SCANCODE_W] && state[SDL_SCANCODE_S]) {
        gevts->move_down = true;
    }
}