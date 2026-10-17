#pragma once

#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "utils.h"
#include "timer.h"
#include "i8254.h"

/**
* @brief Initializes graphic mode
*
* Initializes graphic mode desired
*
* @param mode - hex mode to initialize
* @return Returns a void pointer to the video memory address space
*/
void* (vg_init)(uint16_t mode);
/**
* @brief Graphic mode preparations
*
* Graphic mode preparations
*
* @param mode - hex mode to initialize
* @return Returns 0 on success, 1 otherwise 
*/
int mode_setup(uint16_t mode);
/**
* @brief Draws a rectangle
*
* Draws a rectangle by coloring correct pixels
*
* @param x - x starting drawing coordinate 
* @param y - y starting drawing coordinate
* @param width - width of the rectangle
* @param height - height of the rectangle
* @param color - hex color of the rectangle
* @return Returns 0 on success, 1 otherwise
*/
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
* @brief Draws a horizontal line
*
* Draws a horizontal line by coloring correct pixels
*
* @param x - x starting drawing coordinate
* @param y - y starting drawing coordinate
* @param len - lenght of the rectangle
* @param color - hex color of the rectangle
* @return Returns 0 on success, 1 otherwise
*/
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
* @brief Fills a pixel
*
* Paints the pixel in coordinates (x,y) with specified color
*
* @param x - x coordinate of the pixel
* @param y - y coordinate of the pixel
* @param color - hex color of the rectangle
* @return Returns 0 on success, 1 otherwise
*/
int fill_pixel(uint16_t x, uint16_t y, uint32_t color);
/**
* @brief Gets the horizontal screen resolution
*
* Gets the horizontal screen resolution
*
* @return Returns the horizontal resolution
*/
uint16_t get_hres();
/**
* @brief Gets the vertical screen resolution
*
* Gets the vertical screen resolution
*
* @return Returns the vertical resolution
*/
uint16_t get_vres();
/**
* @brief Draws the xpm
*
* Paints the given xpm to the screen
*
* @param xpm_adr - pointer to the xpm's starting address
* @param img - image in the type xpm_image_t
* @param x - x coordinate
* @param y - y coordinate
* @return Returns 0 on success, 1 otherwise
*/
int draw_xpm(uint8_t* xpm_adr, xpm_image_t img, uint16_t x, uint16_t y);
/**
* @brief Updates values
*
* Updates given xi and xf with the given value until they reach xf and yf respectively
*
* @param value - value to increment
* @param xi - pointer to the starting x coordinate
* @param yi - pointer to the starting y coordinate
* @param xf - finishing x coordinate
* @param yf - finishing x coordinate
* @return Returns nothing
*/
void increment(int16_t value, uint16_t *xi, uint16_t *yi, uint16_t xf, uint16_t yf);
/**
* @brief Clears screen
*
* Clears screen with memset
* @return Returns nothing
*/
void clear();
/**
* @brief Clears double buffer
*
* Clears double buffer with memset
* @return Returns nothing
*/
void clear_buffer();
/**
* @brief Draws buffer
*
* Draws whatever is in the buffer to the screen
* @return Returns nothing
*/
void draw_buffer();
/**
* @brief Gets double buffer
*
* Gets the double buffer
*
* @return Returns a void pointer to the double buffer
*/
void * getBuffer();
/**
* @brief Gets double buffer's size
*
* Gets double buffer's size
*
* @return Returns an unsigned int with double buffer's size
*/
size_t getSize();
