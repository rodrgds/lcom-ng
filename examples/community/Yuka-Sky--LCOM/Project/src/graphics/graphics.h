#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include "VBE.h"
#include <lcom/lcf.h>

#define BACKGROUND_COLOR 0x084C61
#define ACCENT_COLOR 0x23988C
#define TEXT_SPACE 0x17645C
#define DRAWING_AREA_COLOR 0xDCF7F2

/**
 * @brief Initializes graphics mode and sets up double buffering.
 * 
 * @param mode the VBE mode to set.
 * @return Pointer to first frame buffer on success or NULL if it fails.
 */
void* (vg_init)(uint16_t mode);

/**
 * @brief Maps frame buffer memory for graphics mode.
 * 
 * @param mode VBE mode to set.
 * @param one_buffer_size pointer to int where the size of one buffer is stored.
 * @return pointer to the mapped video memory or NULL if it fails.
 */
void* (set_frame_buffer)(uint16_t mode, int* one_buffer_size);

/**
 * @brief Draws an horizontal line.
 * 
 * @param x starting coordinate of x.
 * @param y starting coordinate of y.
 * @param len lenght of line.
 * @param color color to use.
 * @return 0 on success.
 * 
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a filled rectangle.
 * 
 * @param x starting coordinate of x.
 * @param y starting coordinate of y.
 * @param width rectangle width.
 * @param height rectangle height.
 * @param color rectangle fill color.
 * @return 0 on success or 1 if draw fails.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a XPM image.
 * 
 * @param img a struct containing the images’s width, height and type information.
 * @param map pointer to image pixel data.
 * @param x int of x position.
 * @param y int of y position.
 * @return 0 on success.
 */
int (vg_draw_xpm)(xpm_image_t img, uint8_t* map, uint16_t x, uint16_t y);

/**
 * @brief Switches display to current off-screen buffer.
 * 
 * @return 0 on success or 1 if BIOS interrupt fails.
 */
int (change_display_buffer)();

/**
 * @brief Clears off-screen buffer with background color.
 * 
 * @return return 0 on success.
 */
int (clear_buffer)();

/**
 * @brief Draws a rectangular block of pixels from a given buffer to the off-screen buffer.
 *
 * @param x int of coordinate x of top left.
 * @param y int of coordinate y of top left.
 * @param width Width of the area.
 * @param height Height of the area.
 * @param map Pointer to pixel data.
 * @return 0 on success, 1 if map is NULL.
 */
int (vg_draw_buffer)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t* map);

#endif /* _LCOM_GRAPHICS_H_ */
