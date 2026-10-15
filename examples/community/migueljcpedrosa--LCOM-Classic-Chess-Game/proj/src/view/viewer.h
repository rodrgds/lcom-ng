/**
 * @file viewer
 * @brief This file has the implementation of the functions related to the viewer of the game.
 */

#ifndef _VIEWER_H_
#define _VIEWER_H_

#include "../drivers/video_card/gpu.h"
#include "../drivers/video_card/vbe.h"
#include "../drivers/rtc/rtc.h"
#include "sprite.h"

#define VBE_MODE MODE5

/**
 * @brief Function to draw a sprite at given coordinates.
 * 
 * @param sprite The sprite to be drawn.
 * @param x The x-coordinate at which the sprite should be drawn.
 * @param y The y-coordinate at which the sprite should be drawn.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (draw_sprite)(Sprite* sprite, uint16_t x, uint16_t y);

/**
 * @brief Function to render the screen.
 * 
 * This function copies the buffer to the video memory to display the current screen.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (render_screen)();

/**
 * @brief Function to draw a cursor on the screen.
 * 
 * This function draws the cursor at the current cursor position.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (draw_cursor)();

/**
 * @brief Function to erase the cursor from the screen.
 * 
 * This function redraws a section of the screen to erase the cursor.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (erase_cursor)();

/**
 * @brief Function to erase a date from the screen.
 * 
 * This function redraws a section of the screen to erase the date.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (erase_date)();

/**
 * @brief Function to draw a word on the screen at given coordinates.
 * 
 * @param sprite The sprite representing the font to use.
 * @param x The x-coordinate at which the word should be drawn.
 * @param y The y-coordinate at which the word should be drawn.
 * @param word The word to be drawn.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (draw_word)(Sprite* sprite, uint16_t x, uint16_t y, char* word);

/**
 * @brief Function to draw a date on the screen.
 * 
 * @param timestamp The date and time to draw.
 * 
 * @return Returns 0 on success, 1 otherwise.
 */
int (drawDate)(rtc_timestamp_t* timestamp);

#endif
