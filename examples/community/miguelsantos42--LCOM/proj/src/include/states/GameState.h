#ifndef _LCOM_GAME_STATE_H_
#define _LCOM_GAME_STATE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/*!
 * @enum GameState
 * @brief Represents the possible states of the game.
 *
 * This enumeration defines the various states the game can be in, such as the main menu,
 * running gameplay, stopping, instructions screen, or settings menu.
 */
enum GameState{
    GameMenu,         /*!< The game is displaying the main menu. */
    GameRunning,      /*!< The game is currently running (gameplay). */
    GameStopping,     /*!< The game is stopping or exiting. */
    GameInstructions, /*!< The game is displaying the instructions screen. */
    GameSettings,     /*!< The game is displaying the settings menu. */
};

/*!
 * @var enum GameState gameState
 * @brief Global variable holding the current state of the game.
 *
 * This variable should be used to check or update the current state of the game.
 * It is initialized to GameMenu at program start.
 */
extern enum GameState gameState;

#endif
