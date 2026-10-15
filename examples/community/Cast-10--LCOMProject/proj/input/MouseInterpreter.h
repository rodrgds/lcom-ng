#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Mouse attack state for a single player.
 */
typedef struct {
    bool attack; /**< True only on the frame the button is first pressed. */
    bool attack_pressed; /**< True while the button is held down. */
} PlayerMouseInput;

/**
 * @brief Combined mouse state for both players.
 */
typedef struct {
    int x; /**< Current cursor X coordinate in pixels. */
    int y; /**< Current cursor Y coordinate in pixels. */
    PlayerMouseInput p1; /**< Player 1 input (left mouse button). */
    PlayerMouseInput p2; /**< Player 2 input (right mouse button). */
} MouseState;

/**
 * @brief Initialises a MouseState with a starting cursor position.
 * @param state MouseState to initialise.
 * @param x Initial cursor X coordinate.
 * @param y Initial cursor Y coordinate.
 */
void mouseStateInit(MouseState *state, int x, int y);

/**
 * @brief Updates a MouseState from a decoded mouse packet.
 * Clamps the cursor to the screen boundaries and converts button events to attack signals.
 * @param state MouseState to update.
 * @param packet Decoded mouse packet from the hardware interrupt handler.
 * @param screen_w Screen width in pixels (used to clamp cursor X).
 * @param screen_h Screen height in pixels (used to clamp cursor Y).
 */
void mouseInterpretPacket(MouseState *state, struct packet *packet, int screen_w, int screen_h);

/**
 * @brief Clears the one-frame attack flags while preserving the held-button state.
 * @param state MouseState to reset.
 */
void mouseClearEvents(MouseState *state);
