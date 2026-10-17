#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "graphic_macros.h"

/**
 * @brief Initializes the graphics card and updates its mode.
 * 
 */
void *(vg_init)(uint16_t mode);

/**
 * @brief Draws a pixel with the specified color in the input coordinates.
 * 
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a line of pixels with the specified color of lenght len, starting in the point with the input coordinates.
 * 
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle with the specified width and height of pixels that have the input color, starting in the point with the input coordinates.
 * 
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Paints the screen with the input color
 * 
 * @param color Input color.
 * @return int 0 if succeded, 1 otherwise.
 */
int paint_screen(uint32_t color);

/**
 * @brief Draws an XPM image in the point with the input coordinates.
 * 
 */
int (vg_xpm_img_draw)(xpm_image_t* image, uint16_t x, uint16_t y); 

/**
 * @brief Get the hres object
 * 
 * @return unsigned horizontal resolution value.
 */
unsigned get_hres();

/**
 * @brief Get the vres object
 * 
 * @return unsigned vertical resolution value.
 */
unsigned get_vres();

#endif
