#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "src/Model/Position.h"
#include "src/include.h"

/**
 * @file Cursor.h
 * @brief Declares the Cursor structure and related functions for mouse control.
 *
 * Cursor structure is the visual pointer controlled by the player.
 * It includes the position and size of the cursor on screen.
 */

/**
 * @struct Cursor
 * @brief Represents the mouse cursor in the game interface.
 *
 * Stores the cursor's current position and its dimensions.
 */

typedef struct
{
    Position position;
    int16_t width, height;

} Cursor;

/**
 * @brief Creates and initializes a new Cursor.
 *
 * @param position Initial position of the cursor.
 * @param width Width of the cursor in pixels.
 * @param height Height of the cursor in pixels.
 * @return Pointer to the newly created Cursor, or NULL if allocation fails.
 */

Cursor *new_cursor(Position position, int16_t width, int16_t height);

/**
 * @brief Frees memory allocated for a Cursor instance.
 *
 * @param c Pointer to the Cursor to delete.
 */

void delete_Cursor(Cursor *c);

/**
 * @brief Gets the current position of the Cursor.
 *
 * @param c Pointer to the Cursor.
 * @return The position of the cursor.
 */

Position get_position(Cursor *c);

#endif
