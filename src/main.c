#include <stdint.h>
#include "game.h"

/**
 * The program's starting point.
 */
int32_t main(int32_t argc, char** args) {
    SDL_DisplayMode DM;
    if (SDL_GetDesktopDisplayMode(0, &DM) < 0) {
        printf("ERROR %s", SDL_GetError());
        exit(1);
    }
    printf("%d %d\n", DM.w, DM.h);
    exit(0);


    Game* game = init_game(argc, args);
    start_game(game);
    destroy_game(game);
    return 0;
}