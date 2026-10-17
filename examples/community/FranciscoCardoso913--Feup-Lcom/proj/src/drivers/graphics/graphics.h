#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "../../xpm/numbers/0.xpm"
#include "../../xpm/numbers/1.xpm"
#include "../../xpm/numbers/2.xpm"
#include "../../xpm/numbers/3.xpm"
#include "../../xpm/numbers/4.xpm"
#include "../../xpm/numbers/5.xpm"
#include "../../xpm/numbers/6.xpm"
#include "../../xpm/numbers/7.xpm"
#include "../../xpm/numbers/8.xpm"
#include "../../xpm/numbers/9.xpm"
#include "../../xpm/numbers/slash.xpm"
#include "../../xpm/numbers/colon.xpm"

#define VBE_VID_INT 0x10 

#define VBE_MODE_INFO 0x4F01
#define VBE_SET_MODE 0x4F02
#define VBE_GET_MODE 0x4F03
#define VBE_MODE_SUPPORTED 0x4F00
#define VBE_FUNCTION 0x4F

#define VBE_VIDEO_MODE_ID 0x0105
#define VBE_VIDEO_MODE_DC 0x014c

#define SET_VBE_MODE 0x4F02 
#define BIOS_SET_MODE 0x00
#define TEXT_MODE 0x03

#define VBE_LINEAR_BUFFER BIT(14)

#define VBE_MODE_GRAPHICS BIT(10)
#define VBE_MODE_COLOR BIT(8)

/**
 * @brief Sets the video mode.
 * 
 * @param mode The new video mode.
 * @return int Returns 1 if an error occurs, 0 otherwise.
 */
int video_set_mode(uint16_t mode);

/**
 * @brief Sets the video mode to 0x14c.
 * 
 * @return int Returns 1 if an error occurs, 0 otherwise.
 */
int video_mode();

/**
 * @brief Sets the video mode to text mode.
 * 
 * @return int Returns 1 if an error occurs, 0 otherwise.
 */
int text_mode();

/**
 * @brief Maps the video memory.
 * 
 * @param mode The video mode.
 * @return int Returns 1 if an error occurs, 0 otherwise.
 */
int map_memory(uint16_t mode);

/**
 * @brief Draws a pixel at the given position with the given color.
 * 
 * @param x The horizontal position on the screen.
 * @param y The vertical position on the screen.
 * @param color The color of the pixel.
 * @return int Returns 1 if the position is outside the borders of the screen, 0 otherwise.
 */
void video_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Returns the horizontal resolution of the screen.
 * 
 * @return uint16_t The number of horizontal pixels on the screen.
 */
uint16_t get_hres();

/**
 * @brief Returns the vertical resolution of the screen.
 * 
 * @return uint16_t The number of vertical pixels on the screen.
 */
uint16_t get_vres();

/**
 * @brief Clears the buffer where the information of all the pixels to be drawn on the screen is stored.
 */
void clear_buffer();

/**
 * @brief Copies the information from the buffer to the video memory (double buffering technique).
 */
void draw_buffer();

/**
 * @brief Draws an xpm image.
 * 
 * @param x The horizontal position of the image.
 * @param y The vertical position of the image.
 * @return int Returns 1 if it can't load the image, 0 otherwise.
 */
int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Copies the information from the

 buffer used to store the background to the buffer used to store the information that is going to be drawn.
 */
void draw_background();

/**
 * @brief Replaces the pixel in the buffer used to store the information that is going to be drawn with the pixel from the buffer used to store the background.
 * 
 * @param x The horizontal position of the pixel.
 * @param y The vertical position of the pixel.
 * @return 1 if the position given is outside the screen, 0 otherwise
 */
int replace_with_background(uint16_t x, uint16_t y);

/**
 * @brief Clears all the buffers and the video memory.
 * 
 */
void clear_background();

/**
 * @brief Draws a number on the screen.
 * 
 * @param x The horizontal position of the number on the screen.
 * @param y The vertical position of the number on the screen.
 * @param number The number to be drawn.
 * @return int Returns 1 if the number is invalid or if the image corresponding to the number failed to load, 0 otherwise.
 */
int draw_number(uint16_t x, uint16_t y, uint32_t number);

/**
 * @brief Draws special characters.
 * 
 * @param x The horizontal position of the character on the screen.
 * @param y The vertical position of the character on the screen.
 * @return int Returns 1 if the character is invalid or if the image corresponding to the character fails to load, 0 otherwise.
 */
int draw_special_char(uint16_t x, uint16_t y, uint32_t special_char);

/**
 * @brief Loads an xpm image.
 * 
 * @param img The xpm image to load.
 * @return uint32_t* A pointer to the map of pixels of the loaded image.
 */
uint32_t* load_img(xpm_map_t img);

#endif /* __GRAPHICS_H */
