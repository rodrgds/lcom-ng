/**
 * @file gamestate.h
 * @brief Game state and menu option enumerations, and Game struct definition.
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

/**
 * @enum GameState
 * @brief Represents the current state of the game.
 */
typedef enum
{
    GAME_STATE_MENU,         /**< Main menu */
    GAME_STATE_GAME,         /**< In-game */
    GAME_STATE_GAMEOVER,     /**< Game over screen */
    GAME_STATE_LEADERBOARD,  /**< Leaderboard screen */
    GAME_STATE_INSTRUCTIONS, /**< Instructions screen */
    GAME_STATE_EXIT          /**< Exit state */
} GameState;

/**
 * @enum MenuOption
 * @brief Menu options for the main menu.
 */
typedef enum
{
    MENU_START_GAME,         /**< Start the game */
    MENU_OPEN_LEADERBOARD,   /**< Open leaderboard */
    MENU_EXIT_GAME           /**< Exit the game */
} MenuOption;

/**
 * @struct Game
 * @brief Holds the overall game state, including leaderboard and score.
 */
typedef struct
{
    int leaderboard[5];  /**< Top 5 leaderboard scores */
    GameState curState;  /**< Current game state */
    int score;           /**< Current score */
} Game;


#endif
