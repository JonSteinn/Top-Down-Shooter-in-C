#include <stdint.h>
#include "game.h"

/**
 * Function:
 *  main
 * 
 * Purpose:
 *  The program's starting point.
 * 
 * argc: 
 *  The number of arguments 
 * args:
 *  The list of arguments
 * 
 * returns: 
 *  0 on succcess, 1 otherwise.
 */
int32_t main(int32_t argc, char** args) {

    foo();
    Game* game = init_game(argc, args);
    start_game(game);
    destroy_game(game);
    return 0;
}