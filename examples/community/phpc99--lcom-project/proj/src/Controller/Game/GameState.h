#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

/**
 * @file GameState.h
 * @brief Declares the GameState enum used to manage the current state of the game.
 */

/**
 * @enum GameState
 * @brief Represents the different states the game can be in.
 */

typedef enum {
    MENU_STATE,
    GAME_STATE,
    OPTIONS_STATE,
    PAUSE_STATE,
    EXIT_STATE
  } GameState;
#endif
