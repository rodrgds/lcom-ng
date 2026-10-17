#ifndef VIDEO_H
#define VIDEO_H

#include <lcom/lcf.h>
#include "vbe.h"

static vbe_mode_info_t mode_info;
static uint8_t *video_mem;	
static uint8_t *second_buffer;
static uint16_t hres, vres;
static uint8_t bytes_per_pixel;


/**
 * @brief Initializes the VBE to the specified mode and gets access to the vram
 * @param mode VBE mode to set
 * @return 0 upon success, 1 otherwise
*/
int (vg_start)(uint16_t mode);

/**
 * @brief Copies the second buffer to the video memory and clears the second buffer
 * @return 0 upon success, 1 otherwise
*/
int (vg_copy_buffer)();

/**
 * @brief Gets the horizontal resolution of the current mode
 * @return Horizontal resolution of the current mode
*/
uint16_t (get_hres)();

/**
 * @brief Gets the vertical resolution of the current mode
 * @return Vertical resolution of the current mode
*/
int16_t (get_vres)();

/**
 * @brief Gets the number of bytes per pixel of the current mode
 * @return Number of bytes per pixel of the current mode
*/
uint8_t (get_bytes_per_pixel)();

/**
 * @brief Normalizes the color of different modes to the 8-8-8 RGB mode
 * @param color Color to normalize
 * @return 0 upon success, 1 otherwise
*/
void (normalize_color)(uint32_t *color);

/**
 * @brief Draws a pixel in the specified coordinates with the specified color
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color Color of the pixel
 * @return 0 upon success, 1 otherwise
*/
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Displays an xpm in the specified coordinates
 * @param xpm Xpm to display
 * @param x X coordinate of the xpm
 * @param y Y coordinate of the xpm
 * @return 0 upon success, 1 otherwise
*/
int (vg_display_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Erases an xpm in the specified coordinates
 * @param xpm Xpm to erase
 * @param x X coordinate of the xpm
 * @param y Y coordinate of the xpm
 * @return 0 upon success, 1 otherwise
*/
int (vg_erase_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);


#endif
