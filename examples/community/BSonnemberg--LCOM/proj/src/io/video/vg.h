#pragma once

#include "../../view/sprite.h"

uint8_t *video_front;
uint8_t *video_back;

uint32_t vsize;

/**
 * @brief Set the color of a screen pixel
 * @param x x-axis coordinate to draw
 * @param y y-axis coordinate to draw
 * @return zero on success, non-0 otherwise
*/
int vg_set_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draw a sprite to the screen
 * @param sp pointer to the target sprite
 * @param x upper-left corner x coord
 * @param y upper-left corner y coord
 * @return zero on success, non-0 otherwise
*/
int vg_draw_sprite(sprite *sp, uint16_t x, uint16_t y);

/**
 * @brief Draw a rotated sprite to the screen
 * @param sp pointer to the target sprite
 * @param x upper-left corner x coord
 * @param y upper-left corner y coord
 * @param angle angle to rotate the sprite by (in radians)
 * @return zero on success, non-0 otherwise
*/
int vg_draw_sprite_rotated(sprite *sp, uint16_t x, uint16_t y, double angle, uint8_t padding);

/**
 * @brief Flip the video buffer, given
 * a double buffer architecture
 * @return zero on success, non-0 otherwise
*/
int vg_flip_buffer();

/**
 * @brief Get screen horizontal resolution
 * @return horizontal resolution (hres)
*/
uint32_t vg_get_hres();

/**
 * @brief Get screen vertical resolution
 * @return vertical resolution (vres)
*/
uint32_t vg_get_vres();
