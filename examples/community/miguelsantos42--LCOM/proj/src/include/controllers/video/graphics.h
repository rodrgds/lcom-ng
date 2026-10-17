#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <stdio.h>
#include <lcom/lcf.h>

/*!
 * @var vbe_mode_info_t mode_info
 * @brief Global variable holding the current VBE mode information.
 */
vbe_mode_info_t mode_info;

/*!
 * @var void* video_mem
 * @brief Pointer to the mapped video memory.
 */
void* video_mem;

/*!
 * @var void* second_buffer
 * @brief Pointer to the secondary buffer used for double buffering.
 */
void* second_buffer;

/*!
 * @brief Sets the video mode.
 *
 * Switches the graphics card to the specified VBE mode.
 *
 * @param[in] mode VBE mode to set.
 * @return 0 on success, non-zero otherwise.
 */
int start_videomode(uint16_t mode);

/*!
 * @brief Exits the current video mode and returns to text mode.
 *
 * @return 0 on success, non-zero otherwise.
 */
int exit_videomode();

/*!
 * @brief Maps the video memory and initializes mode information.
 *
 * @param[in] mode VBE mode to map.
 * @return 0 on success, non-zero otherwise.
 */
int map_memory(uint16_t mode);

/*!
 * @brief Corrects the color value according to the current mode.
 *
 * @param[in] color Input color value.
 * @param[out] new_color Pointer to store the corrected color value.
 * @return 0 on success, non-zero otherwise.
 */
int color_correction(uint32_t color, uint32_t *new_color);

/*!
 * @brief Draws a pixel at the specified coordinates with the given color.
 *
 * @param[in] x X coordinate.
 * @param[in] y Y coordinate.
 * @param[in] color Color value.
 * @return 0 on success, non-zero otherwise.
 */
int draw_pixel(int16_t x, int16_t y, uint32_t color);

/*!
 * @brief Draws a horizontal line starting at (x, y) with the specified width and color.
 *
 * @param[in] x Starting X coordinate.
 * @param[in] y Y coordinate.
 * @param[in] width Width of the line.
 * @param[in] color Color value.
 * @return 0 on success, non-zero otherwise.
 */
int draw_line(int16_t x, int16_t y, uint16_t width, uint32_t color);

/*!
 * @brief Draws a rectangle at (x, y) with the specified width, height, and color.
 *
 * @param[in] x X coordinate of the top-left corner.
 * @param[in] y Y coordinate of the top-left corner.
 * @param[in] width Width of the rectangle.
 * @param[in] height Height of the rectangle.
 * @param[in] color Color value.
 * @return 0 on success, non-zero otherwise.
 */
int draw_rectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);

/*!
 * @brief Draws a pattern of rectangles on the screen.
 *
 * @param[in] mode VBE mode.
 * @param[in] no_rectangles Number of rectangles per row and column.
 * @param[in] first First color value.
 * @param[in] step Color step.
 * @return 0 on success, non-zero otherwise.
 */
int draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

/*!
 * @brief Draws an XPM image at the specified coordinates.
 *
 * @param[in] xpm_image XPM image structure.
 * @param[in] map Pointer to the XPM pixel map.
 * @param[in] x X coordinate.
 * @param[in] y Y coordinate.
 * @return 0 on success, non-zero otherwise.
 */
int draw_xpm(xpm_image_t xpm_image, uint8_t* map, int16_t x, int16_t y);

/*!
 * @brief Loads an XPM image and its pixel map.
 *
 * @param[in] xpm XPM map to load.
 * @param[out] img Pointer to the XPM image structure to fill.
 * @param[out] map Pointer to store the address of the loaded pixel map.
 * @return 0 on success, non-zero otherwise.
 */
int load_xpm(xpm_map_t xpm, xpm_image_t *img, uint8_t** map);

/*!
 * @brief Resets the secondary buffer by clearing its contents.
 */
void reset_buffer();

/*!
 * @brief Copies the contents of the secondary buffer to the video memory.
 *
 * @return 0 on success, non-zero otherwise.
 */
int change_buffer();

/*!
 * @brief Frees the secondary buffer.
 *
 * @return 0 on success, non-zero otherwise.
 */
int free_buffer();

#endif
