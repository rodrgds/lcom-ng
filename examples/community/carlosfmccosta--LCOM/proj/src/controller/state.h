#ifndef __STATE_H
#define __STATE_H

/**
 * @file state.h
 * @brief Defines the possible states of the game.
 */

/**
 * @enum state_t
 * @brief Enumeration of all possible game states.
 *
 * This enum defines the different states the game can be in,
 * such as the main menu, in-game, pause menu, leaderboard, etc.
 */
typedef enum {
  MAIN_MENU,         /**< The main menu is displayed. */
  START_GAME,        /**< Transition state to start a new game. */
  IN_GAME,           /**< The game is actively being played. */
  GO_TO_PAUSE_MENU,  /**< Transition state to pause the game. */
  PAUSE_MENU,        /**< The pause menu is displayed. */
  CONTINUE_GAME,     /**< Transition state to resume the game from pause. */
  GO_TO_LEADERBOARD, /**< Transition state to show the leaderboard. */
  GO_TO_MAIN_MENU,   /**< Transition state to return to the main menu. */
  LEADERBOARD,       /**< The leaderboard is displayed. */
  GAME_OVER,         /**< The game has ended. */
  WRITE_NAME,        /**< The player is entering their name for the leaderboard. */
  EXIT               /**< The game is exiting. */
} state_t;

#endif // __STATE_H

