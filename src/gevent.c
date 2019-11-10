#include "gevent.h"

/**
 * Function:
 *  __set_to_default
 *
 * Purpose:
 *  Sets all boolean fields to false.
 *
 * Parameters:
 *  - gevts:
 *      The GameEvents object.
 *
 * Returns:
 *  Nothing.
 */
static void __set_to_default(GameEvents* gevts);

/**
 * Function:
 *  __poll_events
 *
 * Purpose:
 *  Polls for currently pending events and maps them to our
 *  GameEvents state.
 *
 * Parameters:
 *  - gevts:
 *      The GameEvents object.
 *
 * Returns:
 *  Nothing.
 */
static void __poll_events(GameEvents* gevts);

/**
 * Function:
 *  __keyboard_state
 *
 * Purpose:
 *  Maps keyboard state to our GameEvents state.
 *
 * Parameters:
 *  - gevts:
 *      The GameEvents object.
 *
 * Returns:
 *  Nothing.
 */
static void __keyboard_state(GameEvents* gevts);

/**
 * Allocate memory for the game event. Only really need
 * to set gevts->quit to its default value but still
 * use the __set_to_default function.
 */
GameEvents* init_game_events(void) {
    GameEvents* gevts = (GameEvents*)malloc(sizeof(GameEvents));
    __set_to_default(gevts);
    return gevts;
}

/**
 * Default all values to false, check for exits and then movement and finally
 * updates mouse position and checks if mouse button 1 is down.
 */
void process_events(GameEvents* gevts) {
    __set_to_default(gevts);
    __poll_events(gevts);
    __keyboard_state(gevts);

    // Pointers are set to mouse position, while the masked value that
    // SDL_GetMouseState returns is 1 if mouse button 1 is down, otherwise 0.
    gevts->shoot = SDL_GetMouseState(&gevts->mouseX, &gevts->mouseY) & SDL_BUTTON(1);
}

/**
 * Free memory of GameEvent struct.
 */
void destroy_game_events(GameEvents* gevts) {
    free(gevts);
}

/**
 * Set all booleans to false. Mouse position is
 * set every frame so we don't need a default
 * value for it.
 */
void __set_to_default(GameEvents* gevts) {
    gevts->quit         = false;
    gevts->move_up      = false;
    gevts->move_down    = false;
    gevts->move_left    = false;
    gevts->move_right   = false;
    gevts->shoot        = false;
}

/**
 * The standard SDL event loop, looking for game exits.
 */
static void __poll_events(GameEvents* gevts) {
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

/**
 * A player is moving left if he presses A but not D.
 * A player is moving right if he presses D but not A.
 * A player is moving up if he presses W but not S.
 * A plyer is moving down if he presses S but not W.
 */
static void __keyboard_state(GameEvents* gevts) {
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