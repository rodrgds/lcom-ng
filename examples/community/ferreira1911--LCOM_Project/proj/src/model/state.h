#ifndef STATE_H
#define STATE_H

#include <lcom/lcf.h>

/**
 * @enum GameState
 * @brief Represents the different states of the game.
 */
typedef enum {
    MENU,
    PLAYING_MODE_1,
    PLAYING_MODE_2,
    PLAYING_MODE_3,
    GAME_OVER
} GameState;

extern GameState game_state;

#endif
