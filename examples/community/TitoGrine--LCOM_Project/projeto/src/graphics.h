#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "MACROS.h"
#include "Images.h"
#include "math.h"

/**
 * @brief Returns information on the input VBE mode, including screen
 * dimensions, color depth and VRAM physical address.
 * Initializes packed vbe_mode__info_t structure passed as an address 
 * with the VBE information on the input mode, by calling VBE function 
 * 0x01, Return VBE Mode Information, and copying the ModeInfoBlock 
 * struct returned by that function.
 * 
 * @param mode 					Mode whose information should be returned 
 * @param mode_info 		address of vbe_mode_info_t structure to be initialized
 * 
 * @return int 
 */
int get_mode_info(uint16_t mode, vbe_mode_info_t *mode_info);

/**
 * @brief Paints the pixel in the given x and y coordinates the
 * given color. This paints both in the buffer array as well as the
 * previous array. Does not change the actual values in memory.
 * 
 * @param x 			Pixel's x coordinate
 * @param y 			Pixel's y coordinate
 * @param color 	Color to paint the pixel
 */
void paint_pixel(int x, int y, u_int32_t color);

/**
 * @brief Paints the pixel in the given x and y coordinates the given
 * color. This paint only in the buffer array. Used to paint cursor 
 * pixels, therefore it deals with unpredictable sprites that are not
 * saved in the previous array. Also does not change the actual pixel 
 * value in memory.
 * 
 * @param x 			Pixel's x coordinate
 * @param y 			Pixel's y coordinate
 * @param color 	Color to paint the pixel
 */
void paint_pixel_cursor(int x, int y, u_int32_t color);

/**
 * @brief Used in rainbow easter egg, it reads the color on the screen
 * in a specific location changing the color passed as argument. The
 * location on the screen has a color pallete and when the colors are
 * read sequencialy from top to bottom it appears as a rainbow.
 * 
 * @param color 	Color to be changed for the position in the current loop
 */
void loop_color(int * color);

/**
 * @brief Returns the color on the screen in the pixel with the given
 * coordinates. It reads from the previous array which holds a copy of
 * the screen excluding the cursor. 
 * 
 * @param x 
 * @param y 
 * @return int 
 */
int get_color(uint16_t x, uint16_t y);

/**
 * @brief Simply copies the values in the array buffer into memory.
 * This is meant to be used for double buffering, meaning all changes
 * occur first on the buffer array and only after will they be drawn
 * in memory (i.e the actual screen the user sees), preventing 
 * flickering.
 * 
 */
void double_buffer();

#endif
