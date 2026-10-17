#ifndef VBE_H
#define VBE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialization of VBE with the parameter mode
 *
 * @param mode mode which VBE will be initialized with
 * @return Return 0 upon success and non-zero otherwise
 */
int video_init(uint16_t mode);

/**
 * @brief Draws in the position (j,i) the pixel with color
 *
 * @param j coordinate x of the pixel
 * @param i coordinate y of the pixel
 * @param color color of the pixel
 * @return Return 0 upon success and non-zero otherwise
 */
int vg_draw_pixel(uint16_t j, uint16_t i, uint32_t *color);

/**
 * @brief Draws in the position (j,i) the pixel with color, except if the color is White
 *
 * @param j coordinate x of the pixel
 * @param i coordinate y of the pixel
 * @param color color of the pixel
 * @return Return 0 upon success and non-zero otherwise
 */
int vg_draw_pixel_transparent(uint16_t j, uint16_t i, uint32_t *color);

#endif
