#include <stdint.h>
#include "game.h"

int32_t main(int32_t argc, char** args) {
    Game* game = init_game(argc, args);
    start_game(game);
    destroy_game(game);
    return 0;
}