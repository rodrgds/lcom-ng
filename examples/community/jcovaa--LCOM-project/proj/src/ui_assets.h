/**
 * @file ui_assets.h
 * @brief This file contains the function for getting the XPM representation of a letter.
 */

#ifndef _UI_ASSETS_H_
#define _UI_ASSETS_H_
 
#include <lcom/lcf.h>
#include "drivers/video/graphics.h"
#include "sprite.h"

/**
 * @brief Converts a character to lowercase of it is an uppercase letter.
 * 
 * @param c The character to convert.
 * @return The lowercase version of the character, or the original character if not uppercase.
 */
char my_tolower(char c);

/**
 * @brief Retrieves the sprite corresponding to a given character.
 * 
 * @param c The character to get the sprite.
 * @return Pointer to the corresponding Sprite, or NULL if not found.
 */
Sprite* get_char_sprite(char c);

/**
 * @brief Draws a word on the screen.
 * 
 * @param word The word to be drawn.
 * @param x The x-coordinate where the word starts.
 * @param y The y-coordinate where the word starts.
 * @param spacing The spacing between letters.
 */
void draw_text(const char* word, uint16_t x, uint16_t y, uint16_t spacing);

/**
 * @brief Draws the mouse cursor on at the specified coordinates.
 * 
 * @param x The x-coordinate where the mouse cursor should be drawn.
 * @param y The y-coordinate where the mouse cursor should be drawn.
 */
void draw_mouse_cursor(uint16_t x, uint16_t y);

#endif /* _UI_ASSETS_H_ */
