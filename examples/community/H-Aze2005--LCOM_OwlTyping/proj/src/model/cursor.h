/**
 * @file cursor.h
 * @brief Cursor model and related operations.
 *
 * Declares the [`Cursor`](cursor.h) structure and functions for creating, moving,
 * destroying, drawing the cursor, and checking if the cursor is over specific UI buttons.
 */

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "images/cursor.xpm"
#include "devices/video.h"

/**
 * @struct Cursor
 * @brief Represents the state of the cursor, including its current and previous positions.
 *
 * @var Cursor::prev_cursor_x
 * Previous X position of the cursor.
 * @var Cursor::prev_cursor_y
 * Previous Y position of the cursor.
 * @var Cursor::x
 * Current X position of the cursor.
 * @var Cursor::y
 * Current Y position of the cursor.
 */
typedef struct {
  int16_t prev_cursor_x; /**< Previous X position of the cursor. */
  int16_t prev_cursor_y; /**< Previous Y position of the cursor. */
  int16_t x;             /**< Current X position of the cursor. */
  int16_t y;             /**< Current Y position of the cursor. */
} Cursor;

/**
 * @brief Constructs a new cursor at the specified position.
 * @param x Initial X position.
 * @param y Initial Y position.
 * @return Pointer to the newly created Cursor, or NULL on failure.
 */
Cursor *construct_cursor(int16_t x, int16_t y);

/**
 * @brief Moves the cursor by the specified deltas.
 * @param cursor Pointer to the Cursor to move.
 * @param x Delta X to move.
 * @param y Delta Y to move.
 * @return 0 on success, non-zero on error.
 */
int move_cursor(Cursor *cursor, int16_t x, int16_t y);

/**
 * @brief Frees the memory allocated for the cursor.
 * @param cursor Pointer to the Cursor to destroy.
 */
void destroy_cursor(Cursor *cursor);

/**
 * @brief Draws the cursor on the screen.
 * @param cursor Pointer to the Cursor to draw.
 */
void draw_cursor(Cursor *cursor);

/**
 * @brief Checks if the cursor is over the start button.
 * @param cursor Pointer to the Cursor.
 * @return true if over the start button, false otherwise.
 */
bool on_start_button(Cursor *cursor);

/**
 * @brief Checks if the cursor is over the top-right button.
 * @param cursor Pointer to the Cursor.
 * @return true if over the top-right button, false otherwise.
 */
bool on_top_right_button(Cursor *cursor);

/**
 * @brief Checks if the cursor is over the continue button.
 * @param cursor Pointer to the Cursor.
 * @return true if over the continue button, false otherwise.
 */
bool on_continue_button(Cursor *cursor);

/**
 * @brief Checks if the cursor is over the pause home button.
 * @param cursor Pointer to the Cursor.
 * @return true if over the pause home button, false otherwise.
 */
bool on_pause_home(Cursor *cursor);

/**
 * @brief Checks if the cursor is over the pause exit button.
 * @param cursor Pointer to the Cursor.
 * @return true if over the pause exit button, false otherwise.
 */
bool on_pause_exit(Cursor *cursor);

/**
 * @brief Checks if the cursor is over the 30s button.
 * @param cursor Pointer to the Cursor.
 * @return true if over the 30s button, false otherwise.
 */
bool on_30s_button(Cursor *cursor);

/**
 * @brief Checks if the cursor is over the 60s button.
 * @param cursor Pointer to the Cursor.
 * @return true if over the 60s button, false otherwise.
 */
bool on_60s_button(Cursor *cursor);

/**
 * @brief Checks if the cursor is over the 120s button.
 * @param cursor Pointer to the Cursor.
 * @return true if over the 120s button, false otherwise.
 */
bool on_120s_button(Cursor *cursor);

#endif
