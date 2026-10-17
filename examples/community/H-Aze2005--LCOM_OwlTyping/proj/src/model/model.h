/**
 * @file model.h
 * @brief Application state model and related operations.
 *
 * Declares the [`State`](model.h) enumeration for application states and functions
 * for managing the state and starting the game.
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include "views/views.h"
#include "model/cursor.h"
#include "model/time_selector.h"

/**
 * @enum State
 * @brief Represents the main states of the application.
 *
 * - HOME: Home screen state.
 * - TYPING: Typing test in progress.
 * - PAUSE: Typing test is paused.
 * - STATS: Displaying statistics.
 * - END: Typing test ended.
 */
typedef enum {
    HOME,
    TYPING,
    PAUSE,
    STATS,
    END
} State;

/**
 * @brief Sets the current application state.
 * @param state The new state to set.
 */
void set_state(State state);

/**
 * @brief Starts the game.
 */
void start_game();

#endif
