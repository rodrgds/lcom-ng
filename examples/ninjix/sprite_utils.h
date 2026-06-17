/**
 * @file sprite_utils.h
 * @brief Sprite utilities -- creation, fill, and scaled blitting.
 * @ingroup group_scene
 *
 * Wraps video_gr_ext primitives with convenience functions for creating
 * blank sprites, filling rectangular regions with a solid colour, and
 * nearest-neighbour scaled blits from a source sprite to a destination
 * sprite.
 */

#ifndef PROJ_SPRITE_UTILS_H
#define PROJ_SPRITE_UTILS_H

#include <stdint.h>

#include "video_gr_ext.h"

int sprite_utils_create(vg_sprite_t *sprite, uint16_t width, uint16_t height);
int sprite_utils_fill_rect(vg_sprite_t *sprite, uint16_t x, uint16_t y,
                           uint16_t width, uint16_t height, uint32_t color);
int sprite_utils_draw_scaled(vg_sprite_t *dst, const vg_sprite_t *src,
                             uint16_t src_x, uint16_t src_y,
                             uint16_t width, uint16_t height,
                             uint16_t dst_x, uint16_t dst_y,
                             uint16_t scale);

#endif
