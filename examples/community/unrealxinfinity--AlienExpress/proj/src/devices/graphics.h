#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../macros/includes.h"

/**
 * @brief Sets the current mode to mode passed by argument
 * @param mode The mode to change into
 * @return Return 0 if successfull and 1 if otherwise
 */
int set_graphics_mode(uint16_t mode);
/**
 * @brief Calls functions to set the mode and initialize the frame buffer
 * @param mode The mode to change into
 * @return Return 0 if successfull and 1 if otherwise
 */
int init_graphics(uint16_t mode);
/**
 * @brief Calls functions to set the mode and initialize the frame buffer
 * @param mode The mode to change into
 * @return Return 0 if successfull and 1 if otherwise
 */
int set_frame_buffer(uint16_t mode);
/**
 * @brief Allocate memory space for buffers
 */
void Allocate();
/**
 * @brief Resets value of prev_buffer and passes content from frame_buffer to prev_buffer and vm_buffer
 */
void pass_to_vm_buffer();
/**
 * @brief Passes content from the map buffer to the frame_buffer and resets value of frame_buffer
 * @param mode The mode to change into
 * @return Return 0 if successfull and 1 if otherwise
 */
void pass_map();

/**
 * @brief Draws pixel onto frame_buffer with color and coordinates passed by argument
 * @param x horizontal coordinate of pixel position
 * @param y vertical coordinate of pixel position
 * @param color Value of color to fill the pixel with
 * @return Return 0 if successfull or if position is not on screen or if color is equal to 0xFFFFFE
 */
int draw_pixel(uint16_t x,uint16_t y,uint32_t color);
/**
 * @brief Draws pixel onto map buffer with color and coordinates passed by argument
 * @param x horizontal coordinate of pixel position
 * @param y vertical coordinate of pixel position
 * @param color_32 Value of color to fill the pixel with
 * @return Return 0 if successfull or if position is not on screen or if color is equal to 0xFFFFFE
 */
int draw_pixel_map(uint16_t x, uint16_t y, uint32_t colors_32);
/**
 * @brief Draws pixel onto frame_buffer with color and coordinates passed by argument
 * @param x horizontal coordinate of pixel position
 * @param y vertical coordinate of pixel position
 * @param color_32 Value of color to fill the pixel with
 * @param type Type of color format. If 0 then the coloring is normal format, if 1 then coloring is green and if 4 the coloring is red
 * @return Return 0 if successfull or if position is not on screen or if color is equal to 0xFFFFFE
 */
int draw_pixel_number(uint16_t x, uint16_t y, uint32_t colors_32, int type);


#endif
