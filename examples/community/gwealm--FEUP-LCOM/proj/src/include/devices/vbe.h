#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#define VBE_MODE 0x11A

#define VBE_INTNO 0x10
#define VBE_SET_TEXT_MODE 0x0003
#define VBE_SET_MODE 0x4F02

/**
 * @brief Handles the VBE video memory
 */
typedef struct {
  uint16_t *vmem;
  uint16_t *buffer;

  uint16_t hres, vres;
  uint8_t bytes_per_pixel;

  uint8_t red_pos, red_size;
  uint8_t green_pos, green_size;
  uint8_t blue_pos, blue_size;
} vbe_video_mem;

/**
 * @brief Handles operations to inicialize VBE for usage
 *
 * @return 0 if the operation was successful, non-zero otherwise 
 */
int (vbe_init)();

/**
 * @brief Handles operations to terminate VBE usage
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_cleanup)();

/**
 * @brief Makes the VBE return to text mode
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_set_text_mode)();

/**
 * @brief Sets the VBE to a determined mode
 *
 * @param mode The mode to set the VBE to 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_set_mode)(uint16_t mode);

/**
 * @brief Initializes the memory to be used by the VBE
 * 
 * @param mode The VBE's current mode
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_mem_map)(uint16_t mode);

/**
 * @brief Handles errors issued by the VBE
 * 
 * @param ah The ah register from the VBE
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_handle_errors)(uint8_t ah);

/**
 * @brief Copies the VBE buffer with the local video buffer
 * 
 */
int (vbe_flush)();

/**
 * @brief Fills every pixel on the screen with the same color
 * 
 * @param color The color to fill the screen with
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_fill)(uint16_t color);

/**
 * @brief Colors a pixel on the VBE
 * 
 * @param x The pixel's x position
 * @param y the pixel's y position
 * @param color The desired color
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_draw_pixel)(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Colors a line on the VBE
 * 
 * @param x The leftest pixel's x position
 * @param y the leftest pixel's y position
 * @param len The line's length
 * @param color The desired color
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_draw_line) (uint16_t x, uint16_t y, uint16_t len, uint16_t color);

/**
 * @brief Colors a rectangle on the VBE
 * 
 * @param x The top-leftest pixel's x position
 * @param y the top-leftest pixel's y position
 * @param width The rectangle's width
 * @param height The rectangle's height
 * @param color The desired color
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

/**
 * @brief Draws a XPM on the VBE
 * 
 * @param x The top-leftest pixel's x position
 * @param y the top-leftest pixel's y position
 * @param xpm The previously loaded XPM
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (vbe_draw_xpm)(uint16_t x, uint16_t y, xpm_image_t* xpm);

/**
 * @return The screen's width
 * 
 */
uint16_t (vbe_get_width)();

/**
 * @return The screen's height
 * 
 */
uint16_t (vbe_get_height)();
