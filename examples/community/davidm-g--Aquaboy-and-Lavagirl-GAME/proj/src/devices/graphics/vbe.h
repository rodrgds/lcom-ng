#include "video.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#define DIRECT_COLOR 0x06

/**
 * @brief Draws a pixel at the specified coordinates with the given color.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return 0 on success, non-zero otherwise.
 */
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Gets the horizontal resolution of the screen.
 *
 * @return The horizontal resolution.
 */
uint16_t get_hres();

/**
 * @brief Gets the vertical resolution of the screen.
 *
 * @return The vertical resolution.
 */
uint16_t get_vres();

/**
 * @brief Draws a horizontal line starting from the specified coordinates with the given length and color.
 *
 * @param x The x-coordinate of the starting point of the line.
 * @param y The y-coordinate of the starting point of the line.
 * @param len The length of the line.
 * @param color The color of the line.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle starting from the specified coordinates with the given width, height, and color.
 *
 * @param x The x-coordinate of the starting point of the rectangle.
 * @param y The y-coordinate of the starting point of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Maps the physical video memory to the virtual address space using the specified video mode.
 *
 * @param mode The video mode to use.
 */
void map_phys_virt(uint16_t mode);

/**
 * @brief Calculates the first red component of a pixel in indexed mode.
 *
 * @param first The first pixel value.
 * @return The first red component.
 */
uint32_t first_red(uint32_t first);

/**
 * @brief Calculates the first green component of a pixel in indexed mode.
 *
 * @param first The first pixel value.
 * @return The first green component.
 */
uint32_t first_green(uint32_t first);

/**
 * @brief Calculates the first blue component of a pixel in indexed mode.
 *
 * @param first The first pixel value.
 * @return The first blue component.
 */
uint32_t first_blue(uint32_t first);

/**
 * @brief Calculates the pixel value in indexed mode based on the row, column, number of rectangles, first pixel value, and step size.
 *
 * @param row The row index.
 * @param col The column index.
 * @param no_rectangles The number of rectangles.
 * @param first The first pixel value.
 * @param step The step size.
 * @return The pixel value.
 */
uint32_t index_indexed_mode(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Calculates the pixel value in direct mode based on the red, green, and blue components.
 *
 * @param red The red component.
 * @param green The green component.
 * @param blue The blue component.
 * @return The pixel value.
 */
uint32_t direct_mode(uint32_t red, uint32_t green, uint32_t blue);

/**
 * @brief Calculates the red component of a pixel in indexed mode based on the first red component, column number, and step size.
 *
 * @param first_red The first red component.
 * @param col_num The column number.
 * @param step The step size.
 * @return The red component.
 */
uint32_t red_value(uint32_t first_red, uint16_t col_num, uint8_t step);

/**
 * @brief Calculates the green component of a pixel in indexed mode based on the first green component, row number, and step size.
 *
 * @param first_green The first green component.
 * @param row_num The row number.
 * @param step The step size.
 * @return The green component.
 */
uint32_t green_value(uint32_t first_green, uint16_t row_num, uint8_t step);

/**
 * @brief Calculates the blue component of a pixel in indexed mode based on the first blue component, row number, column number, and step size.
 *
 * @param first_blue The first blue component.
 * @param row_num The row number.
 * @param col_num The column number.
 * @param step The step size.
 * @return The blue component.
 */
uint32_t blue_value(uint32_t first_blue, uint16_t row_num, uint16_t col_num, uint8_t step);

/**
 * @brief Prints an XPM image at the specified coordinates.
 *
 * @param xpm The XPM image to print.
 * @param x The x-coordinate of the starting point of the image.
 * @param y The y-coordinate of the starting point of the image.
 * @return 0 on success, non-zero otherwise.
 */
int print_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Copies the buffer to the screen.
 */
void buffer_copy();

/**
 * @brief Flips the screen by swapping the front and back buffers.
 */
void flip_screen();
