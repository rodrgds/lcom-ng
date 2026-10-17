#ifndef __VIDEO_H
#define __VIDEO_H

#include <lcom/lcf.h>

/**
 * @defgroup video Video
 * @{
 *
 * Functions and structures for interacting with the video mode
 */

/**
 * @brief Structure containing information about the video mode
 */
vbe_mode_info_t mode_info;

/**
 * @brief Pointer to the video memory
 */
char *video_mem;

/**
 * @brief X resolution of the screen
 */
uint32_t XResolution;

/**
 * @brief Y resolution of the screen
 */
uint32_t YResolution;

/**
 * @brief Number of bytes per pixel
 */
uint32_t BytesPerPixel;

/**
 * @brief Sets the video mode using VBE
 *
 * @param submode VBE submode to set
 * @return 0 upon success, non-zero otherwise
 */
int (set_video_mode)(uint16_t submode);

/**
 * @brief Sets the frame buffer to the specified mode
 *
 * @param mode Video mode to set
 * @return 0 upon success, non-zero otherwise
 */
int (set_frame_buffer)(uint16_t mode);

/**
 * @brief Clears the frame buffer by setting all pixels to black
 */
void (clear_frame_buffer)();

/**
 * @brief Normalizes a color to match the frame buffer's format
 *
 * @param color Color to normalize
 * @param normalized_color Pointer to store the normalized color
 */
void (normalize_color)(uint32_t color, uint32_t *normalized_color);

/**
 * @brief Draws a pixel of the specified color at the given coordinates
 *
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color Color of the pixel
 */
void (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line of the specified color starting at the given coordinates
 *
 * @param x X coordinate of the starting point
 * @param y Y coordinate of the starting point
 * @param len Length of the line
 * @param color Color of the line
 * @return 0 upon success, non-zero otherwise
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a vertical line of the specified color starting at the given coordinates
 *
 * @param x X coordinate of the starting point
 * @param y Y coordinate of the starting point
 * @param len Length of the line
 * @param color Color of the line
 * @return 0 upon success, non-zero otherwise
 */
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle of the specified color starting at the given coordinates
 *
 * @param x X coordinate of the starting point
 * @param y Y coordinate of the starting point
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle
 * @return 0 upon success, non-zero otherwise
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Converts row and column coordinates to an index in indexed mode
 *
 * @param col Column coordinate
 * @param row Row coordinate
 * @param step Number of bits for each color component
 * @param first Index of the first color
 * @param n Number of colors
 * @return The index corresponding to the specified coordinates
 */
unsigned (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);

/**
 * @brief Converts RGB components to a direct color in RGB565 mode
 *
 * @param R Red component (5 bits)
 * @param G Green component (6 bits)
 * @param B Blue component (5 bits)
 * @return The direct color value
 */
unsigned (direct_mode)(uint32_t R, uint32_t G, uint32_t B);

/**
 * @brief Extracts the red component from a direct color
 *
 * @param col Direct color value
 * @param step Number of bits for each color component
 * @param first Index of the first color
 * @return The red component value
 */
unsigned (Red)(unsigned col, uint8_t step, uint32_t first);

/**
 * @brief Extracts the green component from a direct color
 *
 * @param row Row coordinate
 * @param step Number of bits for each color component
 * @param first Index of the first color
 * @return The green component value
 */
unsigned (Green)(unsigned row, uint8_t step, uint32_t first);

/**
 * @brief Extracts the blue component from a direct color
 *
 * @param row Row coordinate
 * @param col Column coordinate
 * @param step Number of bits for each color component
 * @param first Index of the first color
 * @return The blue component value
 */
unsigned (Blue)(unsigned row, unsigned col, uint8_t step, uint32_t first);

/**
 * @brief Extracts the red component from a direct color
 *
 * @param color Direct color value
 * @return The red component value
 */
unsigned (R)(uint32_t color);

/**
 * @brief Extracts the green component from a direct color
 *
 * @param color Direct color value
 * @return The green component value
 */
unsigned (G)(uint32_t color);

/**
 * @brief Extracts the blue component from a direct color
 *
 * @param color Direct color value
 * @return The blue component value
 */
unsigned (B)(uint32_t color);

/**
 * @brief Swaps the buffers to update the screen
 */
void (swap_buffers)();

/**
 * @brief Custom function to exit the video graphics module
 *
 * @return 0 upon success, non-zero otherwise
 */
int (custom_vg_exit)();

/**
 * @brief Prints an XPM image on the screen at the specified coordinates
 *
 * @param info XPM image information
 * @param img Pointer to the image data
 * @param x X coordinate of the top-left corner of the image
 * @param y Y coordinate of the top-left corner of the image
 * @return 0 upon success, non-zero otherwise
 */
int (print_xpm)(xpm_image_t info, uint8_t *img, uint16_t x, uint16_t y);

#endif /* __VIDEO_H */
