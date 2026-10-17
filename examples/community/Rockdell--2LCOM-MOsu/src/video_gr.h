#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "sprite.h"
#include "circle.h"

/** @defgroup video_gr Video graphics
 *  @{
 *
 *  Functions for outputing data to screen in graphics mode
 */

/**
 *  @brief Initializes the video module in graphics mode
 * 
 *  Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 *  @param mode 16-bit VBE mode to set
 *  @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 *  @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 *  @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
 *  @brief Draws a circle according to the parameters.
 *  @param x0 X-position of the circle.
 *  @param y0 Y- position of the circle.
 *  @param radius Radius of the circle.
 *  @param color Color of the circle.
 */
void vg_circle(unsigned short x0, unsigned short y0, unsigned short radius, unsigned long color);

/**
 *  @brief Paints a pixel with a certain color.
 *  @param x X-position of the pixel.
 *  @param y Y-position of the pixel.
 *  @param color Color of the pixel.
 */
void vg_paintPixel(unsigned short x, unsigned short y, unsigned long color);

/**
 *  @brief Creates a sprite and calls a function to draw it or erase it.
 *  @param xpm XPM to create the sprite from.
 *  @param xi X-position of the sprite.
 *  @param yi Y-postition of the sprite.
 *  @param option 1 to draw the sprite and 0 to erase the sprite.
 */
void vg_sprite(char *xpm[], unsigned short xi, unsigned short yi, int option);

/**
 *  @brief Copies the memory of the secondary video memory to the first video memory.
 *
 *  This function allows to implement double buffering and make the movements on the screen
 *  more fluid.
 */
void vg_nextFrame();

/**
 *  @brief Clears the screen completely.
 *
 *  Need to be careful when using this functions, it may cause flickering.
 */
void vg_clear();

/**
 *  @brief Draws a rectangle with the height given
 *	@param life Height of the rectangle (life of the player)
 */
void vg_drawRectangle(int life);


/** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
