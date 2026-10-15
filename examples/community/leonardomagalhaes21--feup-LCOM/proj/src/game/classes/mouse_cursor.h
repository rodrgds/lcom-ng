/**
 * @file mouse_cursor.h
 * @brief File containing the declaration of the MouseCursor class.
 */

#ifndef MOUSE_CURSOR_H_
#define MOUSE_CURSOR_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "game/sprite/sprite.h"


/**
 * @brief Struct representing the mouse cursor in the game.
 * 
 * Stores information about the cursor's position and associated sprite.
 */
typedef struct 
{
    int x;
    int y;
    Sprite *sprite;
} MouseCursor;


/**
 * @brief Creates a new mouse cursor.
 * 
 * @param x The initial x-coordinate of the mouse cursor.
 * @param y The initial y-coordinate of the mouse cursor.
 * @param sp Pointer to the sprite associated with the mouse cursor.
 * @return A pointer to the newly created mouse cursor.
 */
MouseCursor *createMouseCursor(int x,int y, Sprite *sp);


#endif
