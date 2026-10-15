/**
 * @file graphics
 * @brief Functions for using the video card
 */

#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

/** @defgroup Video_Card Video_Card
 * @{
 *
 * Functions for using video card
 */

#include <lcom/lcf.h>
#include <stdint.h>

#include "vbe.h"

static uint8_t *frame_adress;
static uint8_t *back_buffer;

static unsigned int h_res;
static unsigned int v_res;
static unsigned int bytes_per_pixel;
vbe_mode_info_t mode_info;

/**
 * @brief Sets the graphic mode
 *
 * @param mode The vbe mode to be set
 * @return Return 0 upon success and 1 otherwise
 */
int (set_graphic_mode)(uint16_t mode);

/**
 * @brief Sets text mode
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (set_text_mode)();

/**
 * @brief Maps the video ram
 *
 * @param mode The vbe mode to get the mode information to map
 * @return Return 0 upon success and 1 otherwise
 */
int (map_vram)(uint16_t mode);

/**
 * @brief Draw the pixel in the screen
 *
 * @param x The x position on the screen of the pixel
 * @param y The y position on the screen of the pixel
 * @param color The color of the pixel
 * @return Return 0 upon success and 1 otherwise
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draw an horizontal line on the screen
 *
 * @param x The x position of the left of the horizontal line on the screen
 * @param y The y position of the left of the horizontal line on the screen
 * @param len The length of the line
 * @param color The color of the line
 * @return Return 0 upon success and 1 otherwise
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draw a rectangle on the screen
 *
 * @param x The x position of the upper left corner of the rectangle on the screen
 * @param y The y position of the upper left corner of the rectangle on the screen
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the rectangle
 * @return Return 0 upon success and 1 otherwise
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Displays the xpm image on the screen
 *
 * @param xpm The xpm image to be displayed
 * @param x The x position of the upper left corner of the image
 * @param y The y position of the upper left corner of the image
 * @return Return 0 upon success and 1 otherwise
 */
int (display_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Creates a back buffer for double buffering
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (create_back_buffer)();

/**
 * @brief Copy the information in the back buffer to the frame buffer
 */
void (copy_back_to_frame)();

/**
 * @brief Sets the back buffer to have white color
 */
void (erase_back_buffer)();

/**
 * @brief Destroy the back buffer freeing its memory
 */
void (destroy_back_buffer)();

/**@}*/

#endif
