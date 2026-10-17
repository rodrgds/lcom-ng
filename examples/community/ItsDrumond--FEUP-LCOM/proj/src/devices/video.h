/**
 * @file video.h
 * 
 */

#ifndef _LCOM_VIDEO_H
#define _LCOM_VIDEO_H

#include <lcom/lcf.h>
#include "Graphic.Macros.h"


// graphic
/**
 * @brief This method copies the contents of the secondary video buffer to the primary video buffer.
 * 
 */
void (change_buffer)();

/**
 * @brief This method sets the frame buffer for the video memory
 * 
 * @param mode VBE Mode 
 * @return Returns an int value of zero upon success and one othersise
 */
int (set_frame_buffer)(uint16_t mode);

/**
 * @brief This method draws a colored pixel
 * 
 * @param x The x coordinate of the pixel
 * @param y The y coordinate of the pixel
 * @param color Color of the pixel
 * @return Returns 0
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief This method loads all the xpm images
 * 
 * @return Returns 0
 */
int (vg_load_xpm)();

/**
 * @brief This method draws a xpm image on the screen
 * 
 * @param id The id of the xpm image
 * @param x The X coordinate of the top-left corner of the xpm image
 * @param y The Y coordinate of the top-left corner of the xpm image
 * @return Returns an int value of zero upon success and one othersise
 */
int (vg_draw_xpm) (uint8_t id, uint16_t x, uint16_t y);

/**
 * @brief This method frees the memory allocated to the buffer and to the spm images
 * 
 */
void free_buffer();


#endif
