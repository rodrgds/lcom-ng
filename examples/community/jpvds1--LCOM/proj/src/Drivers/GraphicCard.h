/**

@file graphiccard.h
@brief Header file containing declarations of functions used for graphics manipulation.

*/

#ifndef __GRAPHICCARD_H
#define __GRAPHICCARD_H

/**
@brief Struct containing information about the graphics mode. */
vbe_mode_info_t mode_inf;
/**
@brief Pointer to the front buffer. */
uint8_t* front_buffer;
/**

@brief Pointer to the back buffer. */
uint8_t* back_buffer;

/**

@brief Function that changes the graphics mode to the specified mode.
@param mode The graphics mode to change to.
@return 0 upon success, non-zero otherwise. */ 
int (change_graphics_mode)(uint16_t mode);
/**

@brief Function that draws a rectangle on the screen.
@param x The x coordinate of the upper-left corner of the rectangle.
@param y The y coordinate of the upper-left corner of the rectangle.
@param width The width of the rectangle.
@param height The height of the rectangle.
@param color The color with which to draw the rectangle.
@return 0 upon success, non-zero otherwise. */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**

@brief Function that draws a horizontal line on the screen.
@param x The x coordinate of the start of the line.
@param y The y coordinate of the start of the line.
@param len The length of the line.
@param color The color with which to draw the line.
@return 0 upon success, non-zero otherwise. */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**

@brief Function that maps the video RAM to the process' virtual address space.
@param mode The graphics mode being used.
@return 0 upon success, non-zero otherwise. */
int (map_vram)(uint16_t mode);
/**

@brief Function that gets the 24-bit color from 32 bits.
@param color1 32-bit color.
@param color2 Returns the 24-bit color.
@return 0 upon success, non-zero otherwise. */
int (get_color)(uint32_t color1, uint32_t* color2);
/**

@brief Function that draws a pixel on the screen.
@param x The x coordinate of the pixel.
@param y The y coordinate of the pixel.
@param color The color of the pixel.
@return 0 upon success, non-zero otherwise. */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
@brief Function that normalizes a color.
@param color The color to normalize.
@return The normalized color. */
uint32_t (normalize_color)(uint32_t color);

/**
@brief Function that draws an XPM image on the screen.
@param sprite Pointer to XPM image. 
@param img The image data. 
@param x The x coordinate of the upper-left corner of the image. 
@param y The y coordinate of the upper-left corner of the image.
@return 0 upon success, non-zero otherwise. */
int (draw_xpm)(uint16_t *sprite, xpm_image_t img, uint16_t x, uint16_t y);

/**
@brief Function that switches the front and back buffer, displaying the new frame on the screen.
@return 0 upon success, non-zero otherwise. */ 
int (video_switch)();

/**
@brief Function that clears the back buffer.
@note This function does not clear the front buffer.
@return void */
void (clear_buffer)();

#endif /* __GRAPHICCARD_H */


