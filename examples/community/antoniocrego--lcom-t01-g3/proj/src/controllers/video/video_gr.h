#ifndef _LCOM_VIDEOGR_H_
#define _LCOM_VIDEOGR_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/// @brief Paints a pixel in the frame buffer.
/// @param x X coordinate of the pixel to be painted.
/// @param y Y coordinate of the pixel to be painted.
/// @param color Color of the pixel to be painted.
/// @return Returns 0 on success and 1 otherwise.
int (paint_pixel)(uint16_t x, uint16_t y, uint32_t color);

#endif
