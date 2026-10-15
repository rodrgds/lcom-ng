/**
 * @file modes.h
 * @brief Interface for game mode selection and state management.
 */

#ifndef _LCOM_MODES_H_
#define _LCOM_MODES_H_

#include <lcom/lcf.h>
#include "view/view.h"
#include "model/state.h"
#include "model/player_menu.h"
#include "model/gamemode_menu.h"
#include "model/react_mode.h"
#include "model/guess_mode.h"
#include "model/game_over_menu.h"
#include "model/instructions.h"
#include "view/player_menu_view.h"
#include "view/gamemode_menu_view.h"
#include "view/react_mode_view.h"
#include "view/guess_mode_view.h"
#include "view/game_over_view.h"
#include "view/instructions_view.h"

/// Function pointer type for interrupt and init handlers
typedef void (*HandlerFn)(void);

/// Mouse interrupt handlers for each game mode
extern HandlerFn mouse_ih_array[6];
/// Keyboard interrupt handlers for each game mode
extern HandlerFn keyboard_ih_array[6];
/// Timer interrupt handlers for each game mode
extern HandlerFn timer_ih_array[6];

/// Initialization functions for each game mode
extern HandlerFn mode_init[6];

/**
 * @enum PlayerMode
 * @brief Defines the type of player configuration.
 */
typedef enum {
    ONE_PLAYER,     /**< Single player mode */
    TWO_PLAYERS,    /**< Two players mode */
} PlayerMode;

/**
 * @enum Winner
 * @brief Defines the winner of a round.
 */
typedef enum {
    SOMEONE,        /**< At least one player reacted */
    NO_ONE,         /**< No one reacted */
} Winner;

/**
 * @struct TimeDiff
 * @brief Stores time differences for player reactions.
 */
typedef struct {
    int player1;    /**< Player 1 reaction time in ms */
    int player2;    /**< Player 2 reaction time in ms */
} TimeDiff;

/**
 * @enum GameMode
 * @brief Defines all possible game modes. 
 */
typedef enum {
    PLAYER_MENU,         /**< Initial player selection menu */
    GAMEMODE_MENU,       /**< Game type selection menu */
    REACT_MODE,          /**< Reaction game mode */
    GUESS_MODE,          /**< Guessing game mode */
    INSTRUCTIONS_SCREEN, /**< Instructions screen */
    GAME_OVER,           /**< Game over screen */
    NO                   /**< No game mode selected */
} GameMode;

/**
 * @brief Changes the current game mode.
 * 
 * Updates the game mode and calls the corresponding initializer if needed.
 * 
 * @param new_mode The mode to switch to
 */
void change_mode(GameMode new_mode);

#endif /* _LCOM_MODES_H_ */
