/**
 * @file graphics.h
 * @brief This file contains the function prototypes for the graphics (graphic mode). It includes the functions to set the graphics mode, draw lines and rectangles. It also contains helper functions to manipulate colors and graphics modes.
 */

#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "video.h"

/**
 * @brief Calculates the number of bytes per pixel based on the bits per pixel.
 * 
 * @param bits_per_pixel The number of bits used to represent a pixel.
 * @return The number of bytes required to store a pixel.
 */
unsigned (calculate_bytes_per_pixel)(uint8_t bits_per_pixel);

/**
 * @brief Initializes the graphics mode by setting the specified video mode.
 * 
 * @param graphic_mode The graphics mode to be set.
 * @return 0 on success, non-zero otherwise.
 */
int (initialize_graphics_mode)(uint16_t graphic_mode);

/**
 * @brief Maps the physical address space of the video memory to the process's virtual address space.
 * 
 * @param mode The video mode whose address space is to be mapped.
 * @return 0 on success, non-zero otherwise.
 */
int (map_address_space)(uint16_t mode);

/**
 * @brief Draws a horizontal line on the screen.
 * 
 * @param x The starting x-coordinate of the line.
 * @param y The y-coordinate of the line.
 * @param len The length of the line.
 * @param color The color of the line.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a vertical line on the screen.
 * 
 * @param x The starting x-coordinate of the line.
 * @param y The starting y-coordinate of the line.
 * @param len The length of the line.
 * @param color The color of the line.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle on the screen.
 * 
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle in pixels.
 * @param height The height of the rectangle in pixels.
 * @param color The color of the rectangle.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Allocates memory for the secondary buffer used for off-screen rendering.
 */
void allocate_secondary_buffer();

/**
 * @brief Copies the contents of the secondary buffer to the video frame buffer.
 */
void copy_buffer_to_video();

/**
 * @brief Clears the secondary buffer by setting all its pixels to zero.
 */
void clear_secondary_buffer();

/**
 * @brief Frees the memory allocated for the secondary buffer.
 */
void free_secondary_buffer();

/**
 * @brief Draws a cell outline on the screen.
 * 
 * @param x The x-coordinate of the top-left corner of the cell.
 * @param y The y-coordinate of the top-left corner of the cell.
 * @param cell_width The width of the cell in pixels.
 * @param cell_height The height of the cell in pixels.
 * @param color The color of the cell outline.
 * @return 0 on success, non-zero otherwise.
 */
int draw_cell_outline(uint16_t x, uint16_t y, uint16_t cell_width, uint16_t cell_height, uint32_t color);

/**
 * @brief Draws a grid outline on the screen.
 * 
 * @param grid_rows The number of rows in the grid.
 * @param grid_cols The number of columns in the grid.
 * @param cell_width The width of each cell in pixels.
 * @param cell_height The height of each cell in pixels.
 * @param start_x The x-coordinate of the top-left corner of the grid.
 * @param start_y The y-coordinate of the top-left corner of the grid.
 * @param color The color of the grid outline.
 * @return 0 on success, non-zero otherwise.
 */
int draw_grid_outline(uint16_t grid_rows, uint16_t grid_cols, uint16_t cell_width, uint16_t cell_height, uint16_t start_x, uint16_t start_y, uint32_t color);

/**
 * @brief Sets the background color of the graphics mode to white.
 */
void set_white_background();

#endif /* _LCOM_GRAPHICS_H_ */
