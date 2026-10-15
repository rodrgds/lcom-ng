/**
 * @file gamestate.h
 * @brief Defines the GameState struct and related functions for managing the overall game state.
 *
 * This header provides the definition of the GameState struct, which holds pointers to all major game elements,
 * and function prototypes for creating, destroying, and drawing the game state.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../controllers/gpu/sprites/sprite.h"
#include "elements/apple.h"
#include "elements/snake.h"
#include "score.h"

/**
 * @struct GameState
 * @brief Represents the overall state of the game, including all main elements and settings.
 */
typedef struct {
    Sprite* gamebackground;      /**< Pointer to the background sprite */
    Apple* normalApple;          /**< Pointer to the normal apple */
    Apple* goldenApple;          /**< Pointer to the golden apple */
    Apple* whiteApple;           /**< Pointer to the white apple */
    Apple* darkApple;            /**< Pointer to the dark apple */
    Snake* snakeClassic;         /**< Pointer to the classic snake (classic mode) */
    FreeSnake* snake;            /**< Pointer to the main free-moving snake */
    FreeSnake* opponentSnake;    /**< Pointer to the opponent snake (local multiplayer) */
    FreeSnake* remoteSnake;      /**< Pointer to the remote snake (network multiplayer) */
    Sprite* cursorSprite;        /**< Pointer to the cursor sprite */
    Score* score;                /**< Pointer to the player's score */
    Score* opponentScore;        /**< Pointer to the opponent's score (multiplayer) */
    int option;                  /**< Game mode option */
    bool multiplayer;            /**< True if multiplayer mode is active */
    bool isRemote;               /**< True if remote multiplayer is active */
    Sprite* apple;               /**< Pointer to a generic apple sprite (legacy or UI use) */
} GameState;

/**
 * @brief Creates and initializes a new GameState.
 * @param option Game mode option.
 * @param isMultiplayer True if multiplayer mode.
 * @param isMultiplayerLocal True if local multiplayer.
 * @param isClassic True if classic mode.
 * @param isRemote True if remote multiplayer.
 * @return Pointer to the created GameState.
 */
GameState* createGameState(int option, bool isMultiplayer, bool isMultiplayerLocal, bool isClassic, bool isRemote);

/**
 * @brief Frees all resources associated with the given GameState.
 * @param gs Pointer to the GameState to destroy.
 */
void destroyGameState(GameState* gs);

/**
 * @brief Draws the current game state to the screen.
 * @param gs Pointer to the GameState to draw.
 */
void drawGameState(GameState* gs);

#endif
