#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <lcom/lcf.h>
#include "vbe.h"

/**
 * @file graphics.h
 * 
 * @brief Graphics management
 * 
 * This module provides functions to manage graphics in the game, including setting video modes, drawing pixels, and handling frame buffers.
 * It allows for double buffering to enable smooth rendering and page flipping.
 */

/**
 * @brief Returns the structure that holds the VBE mode information
 * 
 * @return VBE mode information structure
 */
vbe_mode_info_t get_mode_info();

/**
 * @brief Sets the video mode
 * 
 * Panic if the mode is not supported.
 * 
 * @param mode Video mode to set
 */
void set_video_mode(uint16_t mode);

/**
 * @brief Allocates and maps double the needed VRAM for page flipping
 * 
 * Panic if the allocation fails.
 * 
 * @param mode Video mode to know the needed memory
 */
void map_graphics_vram(uint16_t mode);

/**
 * @brief Toggles the front and back VRAM buffers and updates the display start
 */
void vg_flip_buffer();

/**
 * @brief Sets the display start to the given buffer (for page flipping)
 * 
 * @param addr Pointer to the buffer to display
 */
void set_display_start(uint32_t addr);

/**
 * @brief Clears the back buffer
 * 
 */
void clear_back_buffer();


/**
 * @brief Draws a pixel on the current back buffer
 * 
 * Panic if something goes wrong.
 * 
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color Color of the pixel
 */
void draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws an xpm image on the current back buffer
 * 
 * @param xpm_map XPM image pixel map to draw
 * @param x X coordinate of the top left corner
 * @param y Y coordinate of the top left corner
 * @param width Width of the image
 * @param height Height of the image
 * @return 0 on success, 1 on failure
 */
int vg_draw_xpm(uint8_t* xpm_map, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Draws an xpm image on the current back buffer copying the whole image at once
 * 
 * This function draws the xpm image interely, which can be useful for large images.
 * 
 * @param xpm_map XPM image pixel map to draw
 * @param x X coordinate of the top left corner
 * @param y Y coordinate of the top left corner
 * @param width Width of the image
 * @param height Height of the image
 * @return 0 on success, 1 on failure
 */
int vg_draw_xpm_by_image(uint8_t* xpm_map, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Draws an xpm image on the current back buffer by line
 * 
 * This function draws the xpm image line by line, which can be useful for large images.
 * 
 * @param xpm_map XPM image pixel map to draw
 * @param x X coordinate of the top left corner
 * @param y Y coordinate of the top left corner
 * @param width Width of the image
 * @param height Height of the image
 * @return 0 on success, 1 on failure
 */
int vg_draw_xpm_by_line(uint8_t* xpm_map, uint16_t x, uint16_t y, uint16_t width, uint16_t height);


#endif
