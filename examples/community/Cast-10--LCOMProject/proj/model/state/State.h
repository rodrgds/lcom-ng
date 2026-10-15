#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>

#include "GameState.h"
#include "MenuState.h"
#include "ShopState.h"
#include "GameOverState.h"

#include "../../input/KeyboardInterpreter.h"
#include "../../input/MouseInterpreter.h"

/**
 * @brief Top-level game state machine identifier.
 */
typedef enum {
    STATE_MENU, /**< Main menu is displayed. */
    STATE_PLAYING, /**< Match is in progress. */
    STATE_PAUSED, /**< Match is paused and the shop is open. */
    STATE_GAME_OVER  /**< A winner has been declared. */
} StateType;

/**
 * @brief Top-level application state.
 */
typedef struct {
    StateType state; /**< Current state machine node. */
    bool running; /**< False signals the main loop to exit. */
    int winner; /**< Team index of the winning player (set on game over). */
    GameState *game; /**< In-match game state. */
    MenuState *menu; /**< Main menu state. */
    ShopState *shop; /**< Upgrade shop state. */
    GameOverState *gameOver; /**< Game-over screen state. */
} State;

/**
 * @brief Allocates all sub-states and transitions to STATE_MENU.
 * @param state State container to initialise.
 * @return true on success, false if any allocation fails.
 */
bool stateInit(State *state);

/**
 * @brief Frees all sub-states owned by the State container.
 * @param state State to destroy.
 */
void stateDestroy(State *state);

/**
 * @brief Transitions from STATE_MENU to STATE_PLAYING.
 * @param state Application state.
 */
void gameStart(State *state);

/**
 * @brief Transitions from STATE_PLAYING to STATE_PAUSED (opens the shop).
 * @param state Application state.
 */
void gamePause(State *state);

/**
 * @brief Transitions from STATE_PAUSED back to STATE_PLAYING.
 * @param state Application state.
 */
void gameResume(State *state);

/**
 * @brief Transitions to STATE_GAME_OVER and records the winner.
 * @param state Application state.
 * @param winner Team index of the winning player.
 */
void gameOver(State *state, int winner);

/**
 * @brief Sets running to false, causing the main loop to exit cleanly.
 * @param state Application state.
 */
void gameQuit(State *state);

/**
 * @brief Resets the game state and transitions back to STATE_PLAYING.
 * @param state Application state.
 */
void gameRestart(State *state);
