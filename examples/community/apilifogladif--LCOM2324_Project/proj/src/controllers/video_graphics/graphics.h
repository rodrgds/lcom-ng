#ifndef _LCOM_GRAPHICSCARD_H_
#define _LCOM_GRAPHICSCARD_H_

/**
 * @file graphics.h
 * 
 * @brief This file contains the graphics card functions.
*/


/**
 * @brief Swaps the contents of the double buffer with the actual video memory.
 * 
 */
void swap_buffers();

/**
 * @brief Initializes the video graphics mode.
 * 
 * @param mode The video mode to set.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (_vg_init)(uint16_t mode);

/**
 * @brief Enters the specified video mode.
 * 
 * @param mode : The video mode to set.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (enter_video_mode)(uint16_t mode);

/**
 * @brief Draws a pixel on the screen at the specified coordinates.
 * 
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color to set the pixel to.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Gets the bits per pixel for the current video mode.
 * 
 * @return int : The number of bits per pixel.
 */
int (bitsPerPixel)();

/**
 * @brief Prints an XPM image at the specified coordinates.
 * 
 * @param x The x-coordinate of the top-left corner of the image.
 * @param y The y-coordinate of the top-left corner of the image.
 * @param pixmap The XPM image data.
 * @param img The XPM image information.
 */
void (printXPM)(uint16_t x, uint16_t y,  uint8_t* pixmap, xpm_image_t *img);

/**
 * @brief Draws an XPM image at the specified coordinates.
 * 
 * @param xpm The XPM image to draw.
 * @param x The x-coordinate of the top-left corner of the image.
 * @param y The y-coordinate of the top-left corner of the image.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif
