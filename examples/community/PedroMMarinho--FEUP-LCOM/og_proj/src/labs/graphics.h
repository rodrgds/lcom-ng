#pragma once
#include <lcom/lcf.h>

/**
 * @brief Maps memory for the given video mode.
 *
 * This function sets up the memory mapping for the specified video mode.
 * 
 * @param mode The video mode to be set.
 * 
 * @return 0 on success, one otherwise.
 */
int mapMemory(uint16_t mode);

/**
 * @brief Changes the video mode.
 *
 * This function switches the display to the specified video mode.
 * 
 * @param mode The video mode to be set.
 * 
 * @return 0 on success, one otherwise.
 */
int changeMode(uint16_t mode);

/**
 * @brief Swaps the front and back buffers.
 *
 * This function performs double buffering by swapping the buffers, which helps to avoid flickering.
 * 
 * @return 0 on success, one otherwise.
 */
int (swap_buffers)();

/**
 * @brief Draws a horizontal line.
 *
 * This function draws a horizontal line starting at (x, y) with the specified length and color.
 * 
 * @param x The x-coordinate of the start point.
 * @param y The y-coordinate of the start point.
 * @param len The length of the line.
 * @param color The color of the line.
 * @return 0 on success, one otherwise.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a vertical line.
 *
 * This function draws a vertical line starting at (x, y) with the specified length and color.
 * 
 * @param x The x-coordinate of the start point.
 * @param y The y-coordinate of the start point.
 * @param len The length of the line.
 * @param color The color of the line.
 * @return 0 on success, one otherwise.
 */
int vg_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle.
 *
 * This function draws a rectangle with the specified dimensions and color.
 * 
 * @param x The x-coordinate of the top-left corner.
 * @param y The y-coordinate of the top-left corner.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * @return 0 on success, one otherwise.
 */

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a pixel.
 *
 * This function sets the color of a single pixel at (x, y).
 * 
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return 0 on success, one otherwise.
 */
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a pattern.
 *
 * This function draws a pattern with a specified number of rectangles, initial color, and step.
 * 
 * @param no_rectangles Number of rectangles in the pattern.
 * @param first The initial color.
 * @param step The step for the color.
 * @return 0 on success, non-zero otherwise.
 */

int vg_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Extracts the red component from a color.
 *
 * This function retrieves the red component from a 32-bit color value.
 * 
 * @param color The 32-bit color value.
 * @return The red component.
 */
uint8_t R(uint32_t color);

/**
 * @brief Extracts the green component from a color.
 *
 * This function retrieves the green component from a 32-bit color value.
 * 
 * @param color The 32-bit color value.
 * @return The green component.
 */
uint8_t G(uint32_t color);

/**
 * @brief Extracts the blue component from a color.
 *
 * This function retrieves the blue component from a 32-bit color value.
 * 
 * @param color The 32-bit color value.
 * @return The blue component.
 */
uint8_t B(uint32_t color);

/**
 * @brief Draws an XPM image.
 *
 * This function draws an XPM image at the specified position and rotation angle.
 * 
 * @param img The XPM image to draw.
 * @param x The x-coordinate where the image will be drawn.
 * @param y The y-coordinate where the image will be drawn.
 * @param angle The rotation angle of the image.
 * @return 0 on success, one otherwise.
 */

int drawXPMImage(xpm_image_t img, double x, double y, double angle);

/**
 * @brief Clears the canvas.
 *
 * This function clears the entire drawing canvas, usually by setting it to a default(all black) background color.
 * 
 * @return 0 on success, one otherwise.
 */
int (cleanCanvas)();


int drawText(char* text, xpm_image_t* font,uint16_t x, uint16_t y,uint8_t spacing);


