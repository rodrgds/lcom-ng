#ifndef CURSOR_H
#define CURSOR_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Cursor Cursor;

/**
 * @brief Creates cursor state.
 * @return Pointer to the cursor, or NULL on allocation failure.
 */
Cursor *cursor_create(void);

/**
 * @brief Destroys cursor state.
 * @param cursor Cursor created with cursor_create(); NULL is accepted.
 */
void cursor_destroy(Cursor *cursor);

/**
 * @brief Sets cursor position.
 * @param cursor Cursor state.
 * @param x New X coordinate.
 * @param y New Y coordinate.
 */
void cursor_set_position(Cursor *cursor, int16_t x, int16_t y);

/**
 * @brief Sets the current left-button state and records click edges.
 * @param cursor Cursor state.
 * @param pressed True while left button is pressed.
 */
void cursor_set_left_pressed(Cursor *cursor, bool pressed);

/**
 * @brief Returns the cursor X coordinate.
 * @param cursor Cursor state.
 * @return Cursor X coordinate, or 0 for NULL.
 */
int16_t cursor_get_x(const Cursor *cursor);

/**
 * @brief Returns the cursor Y coordinate.
 * @param cursor Cursor state.
 * @return Cursor Y coordinate, or 0 for NULL.
 */
int16_t cursor_get_y(const Cursor *cursor);

/**
 * @brief Returns whether the left button is currently pressed.
 * @param cursor Cursor state.
 * @return true if pressed, false otherwise.
 */
bool cursor_is_left_pressed(const Cursor *cursor);

/**
 * @brief Returns whether the left button was clicked on the last update.
 * @param cursor Cursor state.
 * @return true on a left-button press edge, false otherwise.
 */
bool cursor_was_left_clicked(const Cursor *cursor);

/**
 * @brief Consumes a pending left-button click edge.
 * @param cursor Cursor state.
 * @return true once per left-button press edge, false otherwise.
 */
bool cursor_consume_left_click(Cursor *cursor);

#endif /* CURSOR_H */
