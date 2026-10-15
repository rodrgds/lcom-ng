#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "Breakcodes.h"

/**
 * @brief Directional key state for a single player.
 */
typedef struct {
    bool up; /**< True while the up key is held. */
    bool down; /**< True while the down key is held. */
    bool left; /**< True while the left key is held. */
    bool right; /**< True while the right key is held. */
} PlayerInput;

/**
 * @brief Combined keyboard state for both players and global keys.
 */
typedef struct {
    PlayerInput p1; /**< Player 1 directional input (WASD). */
    PlayerInput p2; /**< Player 2 directional input (arrow keys). */
    bool exit; /**< True while ESC is pressed. */
    bool shop; /**< True while the P key is pressed. */
} KeyboardState;

/**
 * @brief Zeroes all fields in a KeyboardState.
 * @param state KeyboardState to initialise.
 */
void keyboardStateInit(KeyboardState *state);

/**
 * @brief Updates a KeyboardState from a raw scancode byte sequence.
 * Handles both single-byte (player 1) and two-byte extended (player 2) scancodes.
 * @param state KeyboardState to update.
 * @param bytes Pointer to the scancode byte buffer.
 * @param size Number of bytes in the buffer (1 for normal, 2 for extended).
 */
void keyboardInterpretScancode(KeyboardState *state, uint8_t *bytes, uint8_t size);
