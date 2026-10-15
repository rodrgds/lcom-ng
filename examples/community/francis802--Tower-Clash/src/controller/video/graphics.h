/**
 * @file graphics.h
 * 
 * @brief File containing the functions to configure and interecat with the video card
 * and the frame buffer
*/

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"

vbe_mode_info_t mode_info;

/**
 * @brief This function sets the graphic mode to the one specified in the submode. It create a reg86_t struct
 * and fills it with the necessary information to set the graphic mode. Then, it calls sys_int86() to
 * set the graphic mode.
 * 
 * @param submode Submode to be set
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (set_graphic_mode)(uint16_t submode);

/**
 * @brief This function fetches the mode information of the specified mode and uses it to set the frame buffer.
 * It allocates memory for the frame buffer and maps the physical address of the frame buffer to the
 * virtual address of the frame buffer, so that the frame buffer can be accessed and modified.
 * 
 * @param mode Mode to be set
 * @param frame_buffer Address of memory of the pointer to the frame buffer
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (set_frame_buffer)(uint16_t mode, uint8_t** frame_buffer);

/**
 * @brief This function draws a pixel in the specified coordinates with the specified color. It calculates the
 * index of the pixel in the frame buffer and copies the color to the frame buffer.
 * 
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color Color of the pixel
 * @param frame_buffer Pointer to the frame buffer
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer);

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t* frame_buffer);

int (draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t* frame_buffer);

#endif
