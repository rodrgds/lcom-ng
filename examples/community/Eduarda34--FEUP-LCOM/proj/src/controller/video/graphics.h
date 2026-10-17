#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>

/**
 * @brief Sets the graphics mode to the specified VBE mode
 * 
 * @param mode The desired VBE video mode
 * 
 * @return 0 on success, 1 on failure
 */
int (graphic_mode)(uint16_t mode);

/**
 * @brief Initializes the frame buffer for the given graphics mode
 * 
 * @param mode The VBE mode to initialize the framebuffer for
 * 
 * @return 0 on success, 1 on failure
 */
int (frame_buffer)(uint16_t mode);

/**
 * @brief Swaps the back buffer to the video memory
 */
void (swap_buffer)();

/**
 * @brief Draws a filled rectangle on the screen
 * 
 * @param x X coordinate of the top-left corner
 * @param y Y coordinate of the top-left corner
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color to fill the rectangle with
 * 
 * @return 0 on success, 1 on failure
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a horizontal line on the screen
 * 
 * @param x Starting X coordinate
 * @param y Y coordinate
 * @param len Length of the line
 * @param color Color of the line
 * 
 * @return 0 on success, 1 on failure
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a pixel on the screen at the specified coordinates
 * 
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color Color of the pixels
 * 
 * @return 0 on success, 1 on failure
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws an XPM image at a given position on the screen
 * 
 * @param xpm The XPM image map to draw
 * @param x X coordinate of the top-left corner
 * @param y Y coordinate of the top-left corner
 * 
 * @return 0 on success, 1 on failure
 */
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Clears the screen by setting all pixels 
 */
void (clear_screen)();

/**
 * @brief Draws the current score on the screen at the specified position
 * 
 * @param score The score value to display
 * @param x The x-coordinate on the screen where the score should be drawn
 * @param y The y-coordinate on the screen where the score should be draw
 */
void (draw_score)(int score, uint16_t x, uint16_t y);

#endif 
