#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "DODGEM.h"


/**
 * @brief allocate memory for all buffers
 *
 * @param mode to display
 */
void graphicsStart(unsigned short mode);

/**
 * @brief vg_exit()
 */
void graphicsExit();

/**
 * @brief fill video_mem with "color"
 *
 * @param color pretended to fill
 */
void graphics_fill(unsigned long color);

/**
 * @brief set a color of a pixel
 *
 * @param x destiny x coord
 * @param y destiny y coord
 * @param color pretended
 */
void set_pixel(unsigned short x, unsigned short y, unsigned long color);

/**
 * @brief return a pointer to the triple buffer
 *
 * @return a pointer to the triple buffer
 */
short *getTripleBuffer();

/**
 * @brief return a pointer to the double buffer
 *
 * @return a pointer to the double buffer
 */
short *getVideoBuffer();

/**
 * @brief return a pointer to the Video Mem
 *
 * @return a pointer to the Video Mem
 */
short *getVideoMem();




#endif
