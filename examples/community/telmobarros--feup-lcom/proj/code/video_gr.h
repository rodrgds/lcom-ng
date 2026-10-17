/**
 * This module contains all functions to manage graphics device
 * Jorge Vale: 50%
 * Telmo Barros: 50%
 * Relative Weight: 10%
 */
#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <stdint.h>
#include "sprite.h"

#define MIN_CHAR	33
#define MAX_CHAR	126
#define BLACK	0X0000
#define BLUE	0X001F
#define GREEN	0x07E0
#define RED		0xF800
#define WHITE	0xFFFF

char * getPhysicalAdress();
unsigned getH_res();
unsigned getV_res();
uint16_t * getBuffer();

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);
 
/**
 * @brief Sets the color(value) of a pixel(video register)
 *
 * @param x X coordinate of pixel
 * @param y Y coordinate of pixel
 * @param color Color(value) 16 bit to set the pixel
 */
int vg_pixel(unsigned short x, unsigned short y, uint16_t  color);

/**
 * @brief Gets color(value) of a pixel(video register)
 *
 * @param x X coordinate of pixel
 * @param y Y coordinate of pixel
 *
 * @return Return the value of the pixel (16 bit color)
 */
uint16_t vg_get_pixel(unsigned int x,unsigned int y);

/**
 * @brief Draws a sprite in the video buffer
 *
 * @param sprite Sprite to be drawn
 * @param transparent_color Transparent color, 16  bit value to not be drawn
 *
 * @return Return 0 upon success
 */
int vg_sprite(Sprite* sprite, uint16_t transparent_color);

/**
 * @brief Draws an image in the video buffer
 *
 * @param map Pointer to the image to be drawn
 * @param x X coordinate to begin the drawing
 * @param y Y coordinate to begin the drawing
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 *
 * @return Return 0 upon success
 */
int vg_map(uint16_t * map, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

/**
 * @brief Draws an image with transparent color in the video buffer
 *
 * @param x X coordinate to begin the drawing
 * @param y Y coordinate to begin the drawing
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Transparent color, 16  bit value to not be drawn
 *
 * @return Return 0 upon success
 */
int vg_map_transparent(uint16_t * map, unsigned int x, unsigned int y, unsigned int width, unsigned int height, uint16_t color);

/**
 * @brief Draws a string in the video buffer using the font Impact Condensed
 *
 * @param string Char pointer that contains string to draw
 * @param x X coordinate to begin the drawing
 * @param y Y coordinate to begin the drawing
 * @param spacing Space between chararacters
 * @param color Color of the characters
 *
 * @return Return 0 upon success
 */
int vg_string(char * string,unsigned int x, unsigned int y, unsigned int spacing,  uint16_t color);

/**
 * @brief Draws a filled rectangle in the video buffer
 *
 * @param x X coordinate to begin the drawing
 * @param y Y coordinate to begin the drawing
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle
 *
 * @return Return 0 upon success
 */
int vg_rectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, uint16_t color);

/**
 * @brief Draws in the video buffer the counter in mm:ss model
 *
 * @param x X coordinate to begin the drawing
 * @param y Y coordinate to begin the drawing
 * @param counter Number of timer ticks
 *
 * @return Return 0 upon success
 */
int vg_counter(unsigned int x, unsigned int y, unsigned long counter);

/**
 * @brief Copy buffer frame to graphics device
 */
void vg_buffer();

int vg_darker();

int vg_fadein(uint16_t * map);

/** @} end of video_gr */

#endif /* __VIDEO_GR_H */
