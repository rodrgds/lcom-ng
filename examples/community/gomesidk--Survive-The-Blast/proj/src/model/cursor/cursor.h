#ifndef __CURSOR_H
#define __CURSOR_H

#include <lcom/lcf.h>
#include <stddef.h>
#include <stdlib.h>
#include "sprite.h"

/**
 * @file cursor.h
 * 
 * @brief Cursor management
 * 
 * This module provides functions to create, manage, and draw the cursor in the game.
 * It handles the cursor's position, button states, and rendering on the screen.
 */

struct cursor_imp;
typedef struct cursor_imp Cursor;

/**
 * @brief Returns the X coordinate of the cursor
 *
 * @param c Pointer to the cursor
 * @return The X coordinate of the cursor
 */
uint16_t get_cursor_Xpos(Cursor *c);

/**
 * @brief Returns the Y coordinate of the cursor
 *
 * @param c Pointer to the cursor
 * @return The Y coordinate of the cursor
 */
uint16_t get_cursor_Ypos(Cursor *c);

/**
 * @brief Returns the button pressed state of the cursor
 *
 * @param c Pointer to the cursor
 * @param button The button to check (0, 1, or 2)
 * @return True if the button is pressed, false otherwise
 */
bool get_cursor_button_pressed(Cursor *c, uint8_t button);

/**
 * @brief Resets the button pressed state of the cursor
 *
 * @param c Pointer to the cursor
 */
void reset_cursor_button_pressed(Cursor *c);

/**
 * @brief Creates a new cursor with the specified sprite
 *
 * @param img The sprite for the cursor
 * @param screen_width The width of the screen
 * @param screen_height The height of the screen
 * @return Pointer to the created cursor, or NULL if the pixmap is invalid
 */
Cursor *create_cursor(Sprite *img, uint16_t screen_width, uint16_t screen_height);

/**
 * @brief Frees the memory allocated for the cursor
 *
 * @param c Pointer to the cursor to be destroyed
 */
void destroy_cursor(Cursor *c);

/**
 * @brief Draws the cursor on the screen
 *
 * @param c Pointer to the cursor to be drawn
 */
void draw_cursor(Cursor *c);

/**
 * @brief Updates the cursor position based on mouse input
 *
 * @param cursor Pointer to the cursor to be updated
 * @param mouse_packet Pointer to the mouse packet containing the movement data
 */
void update_cursor_with_mouse(Cursor *cursor, struct packet *mouse_packet);

 #endif
 
