/**
 * @file image.h
 *
 */

#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include <lcom/lcf.h>

/**
 * @brief struct that models an image
 * @param type specifies whether the image is a sprite or a simple vectorial shape
 * @param colors contains the color(s) of the image
 */
typedef struct {
    enum {SPRITE, SHAPE} type;
    uint16_t width, height;
    uint16_t *colors;
} Image;

Image *image_create_sprite(xpm_map_t xpm);
Image *image_create_shape(uint16_t width, uint16_t height, uint16_t color);
void image_switch(Image* image, xpm_map_t xpm);
void image_destroy(Image* image);

#endif // _LCOM_SPRITE_H_
