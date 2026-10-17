#ifndef PROJ_MAIN_LOOP_H
#define PROJ_MAIN_LOOP_H

#include <lcom/lcf.h>

/**
 * @brief Enum representing the various states of the game.
 */
typedef enum {
    MENU, /**< The game is in the menu state */
    RECORDS, /**< The game is displaying the records */
    SINGLE_PLAYER, /**< The game is in the single player mode */
    CLOSE /**< The game is closing */
} Game_State;
#define FPS 30 /**< Game frame rate*/

#endif
