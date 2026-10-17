#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdint.h>
#include <stdio.h>
#include <lcom/lcf.h>

#include "../utils.h"

#define VBE_MODE_INDEXED_768 0x105
/// Stores information about the current VBE video mode.
vbe_mode_info_t mode_info;

/// Pointer to the frame buffer (video memory).
uint8_t* frame_buffer;

/// Pointer to the shadow buffer used for double buffering.
uint8_t* shadow_buffer;

/// Total size of the frame buffer in bytes.
unsigned int frame_size;

/**
 * @brief Switches to a VBE graphics mode.
 * @param submode The video mode to set.
 * @return 0 on success, non-zero otherwise.
 */
int set_graphic_mode(uint16_t submode);    

/**
 * @brief Exits graphics mode and returns to text mode.
 * @return 0 on success, non-zero otherwise.
 */
int set_text_mode();   

/**
 * @brief Initializes and maps the frame buffer for graphics rendering.
 * @param mode The VBE mode to be used.
 * @return 0 on success, non-zero otherwise.
 */
int set_frame_buffer(uint16_t mode);  

/**
 * @brief Adjusts a color to match the current bits-per-pixel.
 * @param color Original color.
 * @param nova_cor Pointer to the adjusted color.
 * @return 0 on success, non-zero otherwise.
 */
int normalize_color(uint32_t color, uint32_t *nova_cor);    

/**
 * @brief Draws a filled rectangle on the screen.
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 * @param largura Width of the rectangle.
 * @param altura Height of the rectangle.
 * @param cor Color of the rectangle.
 * @return 0 on success, non-zero otherwise.
 */
int my_vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t largura, uint16_t altura, uint32_t cor);

/**
 * @brief Draws a horizontal line.
 * @param x Starting x coordinate.
 * @param y Y coordinate.
 * @param comprimento Length of the line.
 * @param cor Color of the line.
 * @return 0 on success, non-zero otherwise.
 */
int my_vg_draw_hline(uint16_t x, uint16_t y, uint16_t comprimento, uint32_t cor);

/**
 * @brief Draws a single pixel at the given coordinates.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param cor Color of the pixel.
 * @return 0 on success, non-zero otherwise.
 */
int my_vg_draw_pixel(uint16_t x, uint16_t y, uint32_t cor);

/**
 * @brief Creates a direct color mode pixel value from RGB components.
 * @param R Red component.
 * @param G Green component.
 * @param B Blue component.
 * @return Pixel value in the current video mode.
 */
uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B);  

/**
 * @brief Calculates color in indexed mode based on position and step.
 * @param col Column index.
 * @param row Row index.
 * @param step Step between colors.
 * @param first First color value.
 * @param n Number of colors in the row.
 * @return Pixel color value.
 */
uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n); 

/**
 * @brief Calculates the red component for a pixel.
 * @param col Column index.
 * @param step Step between values.
 * @param first Base color value.
 * @return Red component value.
 */
uint32_t get_red_component(unsigned col, uint8_t step, uint32_t first);

/**
 * @brief Calculates the green component for a pixel.
 * @param row Row index.
 * @param step Step between values.
 * @param first Base color value.
 * @return Green component value.
 */
uint32_t get_green_component(unsigned row, uint8_t step, uint32_t first);

/**
 * @brief Calculates the blue component for a pixel.
 * @param col Column index.
 * @param row Row index.
 * @param step Step between values.
 * @param first Base color value.
 * @return Blue component value.
 */
uint32_t get_blue_component(unsigned col, unsigned row, uint8_t step, uint32_t first);

/**
 * @brief Draws an XPM image at a given position on screen.
 * @param xpm The XPM image.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return 0 on success, non-zero otherwise.
 */
int (draw_xpm_image)(xpm_map_t xpm, uint16_t x, uint16_t y);    

/**
 * @brief Swaps the shadow buffer with the visible frame buffer.
 */
void swap_buffers();
#endif
