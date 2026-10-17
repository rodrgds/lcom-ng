#ifndef _LCOM_GPU_H_
#define _LCOM_GPU_H_

#include <lcom/lcf.h>
#include <math.h>

#define GPU_800x600 0x115 /*!< @brief VideoCard mode */

#define VBE_CALL_FUNCTION_02 0x4F02 /*!< @brief  VBE call, function 02 - set VBE mode */
#define LINEAR_FRAME_BUFFER BIT(14)  /*!< @brief  Use linear frame buffer model */

vbe_mode_info_t vbe_mode_info;
unsigned int frame_size;
uint8_t* frame_memory;
uint8_t* frame_buffer;
int16_t x_res;
int16_t y_res;
static unsigned bytes_per_pixel;

/**
 * @brief Sets a given graphic mode
 * 
 * @param mode Operation mode
 * @return 0 if success, 1 if error
 */
int(set_graphics_mode)(uint16_t mode);

/**
 * @brief Builds a frame buffer for video memory
 * 
 * @param mode Operation mode
 * @return 0 if success, 1 if error
 */
int(build_frame_buffer)(uint16_t mode);

/**
 * @brief Passes the new buffer contents to the main buffer
 * 
 */
void (swap_buffer)();

/**
 * @brief Clears a secondary buffer.
 * 
 */
void (clear_buffer)();

/**
 * @brief Draws a pixel in the given coordinates with the given color
 * 
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color to be used
 * @return 0 if success, 1 if error
 */
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a rectangle with the given dimensions and color, starting in the given coordinates
 * 
 * @param x X coordinate
 * @param y Y coordinate
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color to be used
 * @return 0 if success, 1 if error
 */
int (draw_rect)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws an XPM image on the screen. This function takes an XPM image and draws it on the screen at the specified coordinates.
 * 
 * @param img XPM image to draw.
 * @param x X coordinate of the top-left corner of the image.
 * @param y Y coordinate of the top-left corner of the image.
 * @param flipped If set to 1, the image will be flipped horizontally.
 * 
 * @return Returns 0 on success, or a non-zero value if an error occurred.
 */
int(draw_xpm)(xpm_image_t xpm, uint16_t x, uint16_t y, int flipped);


#endif
