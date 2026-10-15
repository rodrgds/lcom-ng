#ifndef BUTTON_H
#define BUTTON_H

#include <lcom/lcf.h>

#include "sprite.h"
#include "cursor.h"

/**
 * @file button.h
 * @brief Button management in the game
 * 
 * This module provides functions to create, manage, and draw buttons in the game.
 * It handles button creation, drawing, and checking if a button is clicked.
 */

struct button_imp;
typedef struct button_imp Button;

/**
 * @brief Creates a new button with the specified position and sprite
 * 
 * @param x The x-coordinate of the button
 * @param y The y-coordinate of the button
 * 
 * @param sprite Pointer to the sprite associated with the button
 * @return Pointer to the created Button object, or NULL if memory allocation fails
 */
Button *create_button(uint16_t x, uint16_t y, Sprite *sprite);

/**
 * @brief Destroys a Button object, freeing all allocated memory
 * 
 * @param button Pointer to the Button object to be destroyed
 */
void destroy_button(Button *button);

/**
 * @brief Draws the button on the screen
 * 
 * @param button Pointer to the Button object to be drawn
 */
void draw_button(Button *button);

/**
 * @brief Checks if the button is clicked by the cursor
 * 
 * @param button Pointer to the Button object
 * @param cursor Pointer to the Cursor object
 * @return true if the button is clicked, false otherwise
 */
bool is_button_clicked(Button *button, Cursor *cursor);

#endif // BUTTON_H
