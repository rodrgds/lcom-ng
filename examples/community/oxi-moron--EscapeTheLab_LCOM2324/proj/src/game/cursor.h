#ifndef G6_CURSOR_H
#define G6_CURSOR_H

#include <lcom/lcf.h>
#include "game_macros.h"

static uint16_t x = CURSOR_X_START, y = CURSOR_Y_START;

/**
 * @brief Moves the cursor by a certain distance.
 * @param x_delta Distance on the x-axis.
 * @param y_delta Distance on the y-axis.
 */
void cursor_move(int x_delta, int y_delta);

/**
 * @brief Gets the current position of the cursor.
 * @param x_pos Address where the x-axis position will be stored.
 * @param y_pos Address where the y-axis position will be stored.
 */
void cursor_get_position(uint16_t* x_pos, uint16_t* y_pos);

#endif
