#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <stdint.h>
#include <lcom/lab5.h>

#include "vbe.h"

/**
 * @brief Initializes the graphics module
 * 
 * @param mode The graphics mode to set
 * return 0 on success, 1 otherwise
*/
int (set_graphics_mode)(uint16_t mode);

/**
 * @brief Sets the frame buffer
 * 
 * @param mode The graphics mode to set
 * return 0 on success, 1 otherwise
*/
int (set_frame_buffer)(uint16_t mode);

/**
 * @brief Draws a pixel in the screen
 * 
 * @param x The x coordinate of the pixel
 * @param y The y coordinate of the pixel
 * @param color The color of the pixel
 * return 0 on success, 1 otherwise
*/
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Do the page flipping
 * 
 * return 0 on success, 1 otherwise
*/
int (vg_page_flipping)();

/**
 * @brief Get the actual buffer
 * 
 * return The actual buffer
*/
uint8_t* (get_actual_buffer)();

/**
 * @brief Get the vram size
 * 
 * return The vram size
*/
int (get_vram_size)();

/**
 * @brief Draw background
 * 
 * return 0 on success, 1 otherwise
*/
int (draw_background)(uint32_t* map);

#endif /* GRAPHICS_H */
