/**
 * @file graphics.h
 * @brief Header file for graphics related functions and definitions.
 */

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"
#include "../kbc/keyboard.h"
#include <lcom/timer.h>

#define SWAP(a,b) {uint8_t temp = a; a = b; b = temp;}

/**
 * @brief Sets the graphics mode for the video controller.
 *
 * This function sets the graphics mode for the video controller using the specified submode.
 *
 * @param submode The submode to set for the graphics mode.
 * @return Returns 0 on success, 1 on failure.
 */
int (set_graphics)(uint16_t submode);

/**
 * @brief Sets the frame buffer for the video controller.
 *
 * This function sets the frame buffer for the video controller using the specified mode.
 *
 * @param mode The mode to set for the frame buffer.
 * @return Returns 0 on success, 1 on failure.
 */
int (set_frame_buffer)(uint16_t mode);

/**
 * Draws a pixel on the screen at the specified coordinates with the specified color.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @return 0 if the pixel was successfully drawn, 1 otherwise.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * Draws a horizontal line on the screen at the specified coordinates with the specified length and color.
 *
 * @param x The x-coordinate of the line.
 * @param y The y-coordinate of the line.
 * @param len The length of the line.
 * @param color The color of the line.
 * @return 0 if the line was successfully drawn, 1 otherwise.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * Draws a rectangle on the screen at the specified coordinates with the specified width, height, and color.
 *
 * @param x The x-coordinate of the rectangle.
 * @param y The y-coordinate of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * @return 0 if the rectangle was successfully drawn, 1 otherwise.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Normalizes the color based on the current video mode.
 *
 * This function takes a color value and normalizes it based on the current video mode's bits per pixel.
 * If the video mode has 32 bits per pixel, the color value remains unchanged.
 * Otherwise, the color value is masked with a bit mask that corresponds to the number of bits per pixel.
 *
 * @param color The original color value.
 * @param new_color Pointer to store the normalized color value.
 * @return 0 on success, indicating that the color was normalized.
 */
int (normalize_color)(uint32_t color, uint32_t *new_color);

/**
 * @brief Waits for the ESC key to be pressed.
 *
 * This function waits for the ESC key to be pressed before returning.
 *
 * @return 0 on success, 1 on failure.
 */
int(waiting_ESC)();

/**
 * @brief Gets the red component of a color.
 *
 * This function extracts the red component of a color value.
 *
 * @param first The color value.
 * @return The red component of the color value.
 */
uint32_t (R)(uint32_t first);

/**
 * @brief Gets the green component of a color.
 *
 * This function extracts the green component of a color value.
 *
 * @param first The color value.
 * @return The green component of the color value.
 */
uint32_t (G)(uint32_t first);

/**
 * @brief Gets the blue component of a color.
 *
 * This function extracts the blue component of a color value.
 *
 * @param first The color value.
 * @return The blue component of the color value.
 */
uint32_t (B)(uint32_t first);

/**
 * @brief Gets the red component of a color using a formula.
 *
 * This function extracts the red component of a color value using a formula.
 *
 * @param j The j value.
 * @param step The step value.
 * @param first The color value.
 * @return The red component of the color value.
 */
uint32_t(Red)(unsigned j, uint8_t step, uint32_t first);

/**
 * @brief Gets the green component of a color using a formula.
 *
 * This function extracts the green component of a color value using a formula.
 *
 * @param i The i value.
 * @param step The step value.
 * @param first The color value.
 * @return The green component of the color value.
 */
uint32_t(Green)(unsigned i, uint8_t step, uint32_t first);

/**
 * @brief Gets the blue component of a color using a formula.
 *
 * This function extracts the blue component of a color value using a formula.
 *
 * @param j The j value.
 * @param i The i value.
 * @param step The step value.
 * @param first The color value.
 * @return The blue component of the color value.
 */
uint32_t(Blue)(unsigned j,unsigned i, uint8_t step, uint32_t first);

/**
 * @brief Creates a color value using the direct mode.
 *
 * This function creates a color value using the direct mode.
 *
 * @param R The red component.
 * @param G The green component.
 * @param B The blue component.
 * @return The color value.
 */
uint32_t(direct_mode)(uint32_t R, uint32_t G, uint32_t B);

/**
 * @brief Creates a color value using the indexed mode.
 *
 * This function creates a color value using the indexed mode.
 *
 * @param colum The column value.
 * @param row The row value.
 * @param step The step value.
 * @param first The color value.
 * @param n The n value.
 * @return The color value.
 */
uint32_t (indexed_mode)(uint16_t colum, uint16_t row,uint16_t step, uint32_t first, uint8_t n);

/**
 * @brief Prints an xpm image on the screen at the specified coordinates.
 *
 * This function prints an xpm image on the screen at the specified coordinates.
 *
 * @param xpm The xpm image to print.
 * @param x The x-coordinate of the image.
 * @param y The y-coordinate of the image.
 * @return 0 if the image was successfully printed, 1 otherwise.
 */
int(print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif /* __GRAPHICS_H */
