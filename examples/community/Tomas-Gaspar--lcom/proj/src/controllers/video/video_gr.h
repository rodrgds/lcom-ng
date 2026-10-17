#ifndef _VIDEO_GR_H_
#define _VIDEO_GR_H_

#include <stdint.h>

/**
 * @ingroup Device_Drivers
 * @{
 * 
 * @ingroup video
 * @{
*/

/**
 * @brief Clears the bits of a color starting from a specified position until the most significant bit.
 *
 * @param color Color to clear the bits from.
 * @param starting_pos Starting bit position to clear.
 * @return Returns the modified color after clearing the bits.
 */
uint8_t (clear_bits)(uint8_t color, uint8_t starting_pos);

/**
 * @brief Draws an XPM image to one of the buffers.
 * 
 * This function draws an XPM image, placing the top-left corner of the image
 * on the specificed coordinates.
 *
 * @param img XPM image to be drawn.
 * @param x X-coordinate of the top-left corner of the image.
 * @param y Y-coordinate of the top-left corner of the image.
 * @param secondBuf Specifies whether to draw on the second frame buffer or not.
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (vg_draw_xpm)(xpm_image_t* img, int16_t x, int16_t y, bool secondBuf);

/**
 * @brief Draws a point to the secondary buffer.
 * 
 * This function draws a filled circle with the specified size and color, 
 * starting at the specified coordinates.
 *
 * @param x X-coordinate of the point.
 * @param y Y-coordinate of the point.
 * @param color Color of the point.
 * @param size Size of the point (width and height).
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (vg_draw_point)(uint16_t x, uint16_t y, uint32_t color, uint8_t size);

/**
 * @brief Draws a pixel to one of the buffers.
 *
 * This function draws a pixel with the specified color, 
 * starting at the specified coordinates.
 *
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @param color Color of the pixel.
 * @param secondBuf Specifies whether to draw on the second frame buffer or not.
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, bool secondBuf);

/**
 * @brief Draws a line to the secondary buffer.
 *
 * This function draws a line with the specified size color, 
 * starting at (x0, y0) and ending at (x1, y1).
 * 
 * @param x0 Starting X-coordinate of the line.
 * @param y0 Starting Y-coordinate of the line.
 * @param x1 Ending X-coordinate of the line.
 * @param y1 Ending Y-coordinate of the line.
 * @param color Color of the line.
 * @param size Size of the line (width and height).
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (vg_draw_line)(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, uint8_t size);

/**
 * @brief Copies the contents of the second buffer to the main buffer.
 */
void (swap_buffers)();

/**
 * @brief Initializes the frame buffers for double buffering.
 *
 * This function intializes the video module to graphics mode and
 * creates the second buffer.
 * 
 * @param mode Video mode to initialize the buffers for.
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (init_buffers)(uint16_t mode);

/**
 * @brief Frees the memory allocated for the second frame buffer.
 *
 * @return Returns 0 upon success, non-zero otherwise.
 */
int (free_second_buffer)();

/**
 *  }@
 * }@
 */

#endif
