#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <stdint.h>

#include "sprite.h"

/** @defgroup cursor cursor
 * @{
 *
 * Functions related to the cursor
 */

/**
 * @brief Struct for the cursor
 */
typedef struct {
  int16_t x;      /*!< horizontal coordinate of the cursor */
  int16_t y;      /*!< vertical coordinate of the cursor */
  Sprite *sprite; /*!< sprite of the cursor */
} Cursor;

/**
 * @brief Cursor constructor
 *
 * @param x horizontal coordinate of the cursor
 * @param y vertical coordinate of the cursor
 * @return Constructed cursor on success, NULL otherwise
 */
Cursor *construct_cursor(int16_t x, int16_t y);

/**
 * @brief Updates cursor position
 *
 * @param cursor pointer to the cursor to be updated/moved
 * @param x new horizontal coordinate of the cursor
 * @param y new vertical coordinate of the cursor
 * @return Return 0 upon success and non-zero otherwise
 */
int move_cursor(Cursor *cursor, int16_t x, int16_t y);

/**
 * @brief Cursor destructor
 *
 * @param cursor pointer to the cursor to be destroyed
 */
void destroy_cursor(Cursor *cursor);

/**@}*/

#endif /* _Cursor_H_ */
