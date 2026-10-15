/**
 * @file entity.h
 * @brief Defines and manages the cursor entity used for mouse interaction.
 */

#ifndef PROJ_ENTITY_H
#define PROJ_ENTITY_H

#include <stdbool.h>
#include <lcom/lcf.h>

#include "menu.h"
#include "game.h"
#include "buttons.h"

typedef struct {
    unsigned int pos_x;
    unsigned int pos_y;
    unsigned int width;
    unsigned int height;
    uint8_t *sprite;
    uint8_t *button_sprite;
    xpm_image_t img;
    xpm_image_t button_img;
} Cursor;

/**
 * @brief Creates a new cursor object.
 * Allocates memory for a Cursor struct, loads its XPM sprite, and initializes its position.
 * @param pos_x The initial x-coordinate for the cursor.
 * @param pos_y The initial y-coordinate for the cursor.
 * @param xpm The XPM map (sprite) for the cursor.
 * @return A pointer to the newly created Cursor, or NULL on failure (memory allocation or XPM loading).
 */
Cursor *create_cursor(unsigned int pos_x, unsigned int pos_y, xpm_map_t xpm);

/**
 * @brief Initializes the mouse cursor.
 * Creates the cursor using `create_cursor` with predefined settings.
 * Sets the global `x_max` and `y_max` based on `mode_info`.
 * @return True if the cursor was successfully initialized, false otherwise.
 */
bool (init_cursor)(void);

/**
 * @brief Draws the specified cursor on the screen.
 * @param c Pointer to the Cursor object to draw.
 * @return 0 on success, 1 if the cursor or its sprite is NULL.
 */
int draw_cursor(Cursor *cursor);

/**
 * @brief Updates the mouse cursor's location based on delta movements.
 * Ensures the cursor stays within the screen boundaries defined by `x_max` and `y_max`.
 * @param delta_x The change in x-coordinate.
 * @param delta_y The change in y-coordinate (note: typically inverted from mouse hardware).
 */
void update_mouse_location(int16_t delta_x, int16_t delta_y);

#endif
