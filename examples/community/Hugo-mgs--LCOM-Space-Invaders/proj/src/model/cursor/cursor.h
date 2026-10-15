#pragma once

#include <lcom/lcf.h>
#include "../../sprites/sprite.h"

typedef struct {
    int x, y;
    Sprite *sprite;
} Cursor;


/**
 * Creates a new cursor object with the specified sprite.
 * 
 * @param sprite Pointer to the Sprite to be used for the cursor.
 * @return Pointer to the newly created Cursor, or NULL if allocation fails.
 */
Cursor *createCursor(Sprite *sprite);
 
/**
 * Updates the cursor's position based on mouse packet data.
 *
 * @param cursor Pointer to the Cursor to update.
 * @param mouse_packet Pointer to the mouse packet containing movement data.
 */
void (cursor_update_location)(Cursor *cursor, struct packet *mouse_packet);
 
/**
 * Draws the cursor on the screen at its current position.
 * 
 * @param cursor Pointer to the Cursor to draw.
 * @return 0 on success, non-zero on failure.
 */
int drawCursor(Cursor *cursor);
 
/**
 * Frees the memory allocated for the cursor and its sprite.
 * 
 * @param cursor Pointer to the Cursor to destroy.
 */
void destroyCursor(Cursor *cursor);
