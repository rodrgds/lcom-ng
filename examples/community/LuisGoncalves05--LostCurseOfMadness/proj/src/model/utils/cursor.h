#ifndef CURSOR_H
#define CURSOR_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "assets/xpm/crosshair.xpm"
#include "assets/xpm/cursor.xpm"
#include "drivers/video/gpu.h"
#include "model/utils/sprite.h"

extern uint16_t x_res; /**< X resolution of the screen */
extern uint16_t y_res; /**< Y resolution of the screen */

typedef enum {
    CURSOR_DEFAULT,  /**< Default cursor */
    CURSOR_CROSSHAIR /**< Crosshair cursor */
} CursorMode;

/**
 * @brief Represents a mouse cursor instance.
 *
 * The Cursor struct is opaque.
 */
typedef struct Cursor Cursor;

/**
 * @brief Creates a new cursor with the specified XPM image.
 *
 * @param xpm XPM image used to represent the cursor.
 * @return Pointer to the created Cursor structure.
 */
Cursor *create_cursor(CursorMode mode, int x, int y);

/**
 * @brief Frees the memory associated with the cursor.
 *
 * @param cursor Pointer to the Cursor structure to destroy.
 * @return 0 for success, 1 for failure.
 */
int destroy_cursor(Cursor *cursor);

/**
 * @brief Returns the current X position of the cursor.
 *
 * @param cursor Pointer to the Cursor structure.
 * @return X coordinate of the cursor.
 */
uint16_t cursor_get_x(Cursor *cursor);

/**
 * @brief Returns the current Y position of the cursor.
 *
 * @param cursor Pointer to the Cursor structure.
 * @return Y coordinate of the cursor.
 */
uint16_t cursor_get_y(Cursor *cursor);

/**
 * @brief Updates the cursor's position based on mouse movement deltas.
 *
 * @param cursor Pointer to the Cursor structure.
 * @param dx Horizontal movement delta.
 * @param dy Vertical movement delta.
 * @return 0 for success, 1 for failure.
 */
int cursor_update(Cursor *cursor, double dx, double dy);

/**
 * @brief Draws the cursor to the given frame buffer.
 *
 * @param cursor Pointer to the Cursor structure.
 * @param buffer Pointer to the frame buffer.
 * @return 0 for success, 1 for failure.
 */
int draw_cursor(Cursor *cursor, uint8_t *buffer);

#endif /* CURSOR_H */
