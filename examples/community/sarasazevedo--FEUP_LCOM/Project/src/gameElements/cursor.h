/**
 * @file cursor.h
 * @brief Module for handling cursor in the game.
 */

#ifndef _LCOM_CURSOR_H_
#define _LCOM_CURSOR_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "sprites/projPixmap.h"
#include "sprites/sprite.h"
#include "devices/videocard.h"


/** @defgroup cursor Cursor
 * @{
 *
 * @brief  Cursor related functions
 * 
 */

/**
 * @brief Structure representing a cursor in the game.
 */

typedef struct {
  uint16_t x; /**< X-coordinate of the cursor */
  uint16_t y; /**< Y-coordinate of the cursor */
  Sprite * curr_sprite; /**< Current sprite of the cursor */

} Cursor;

/**
 * @brief Creates a new cursor.
 * 
 * @param sprite XPM map of the sprite for the cursor.
 * @param x Initial X-coordinate of the cursor.
 * @param y Initial Y-coordinate of the cursor.
 * @return Pointer to the created cursor.
 */

Cursor* createCursor(xpm_map_t sprite, uint16_t x, uint16_t y);

/**
 * @brief Draws the cursor on the screen.
 * 
 * @param cursor Pointer to the cursor to be drawn.
 */
void drawCursor(Cursor* cursor);

/**
 * @brief Updates the position of the cursor.
 * 
 * @param cursor Pointer to the cursor to be updated.
 * @param x New X-coordinate of the cursor.
 * @param y New Y-coordinate of the cursor.
 */
void updateCursor(Cursor* cursor, uint16_t x, uint16_t y);

/**
 * @brief Destroys the cursor and frees its memory space.
 * 
 * @param cursor Pointer to the cursor to be destroyed.
 */
void destroyCursor(Cursor* cursor);

/** @} end of cursor */

#endif /* _LCOM_CURSOR_H_ */
