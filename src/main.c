#include <stdint.h>
#include "game.h"

/**
 * The program's starting point.
 */
int32_t main(int32_t argc, char** args) {
    Game* game = init_game(argc, args);
    start_game(game);
    destroy_game(game);
    return 0;
}

/*
some rand strings

okgLyTpWkI
YhFtlVUKnb
wzoKbdm408
oFlVH9GDPt
x9bfTkfyVx
wHBqrj2E14
eOXY0hjAhB
ks2Ye6goo7

*/