#ifndef _LCOM_GPU_H_
#define _LCOM_GPU_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>
#include "src/Model/Elements/Sprite.h"

/**
 * @file GPU.h
 * @brief Declares low-level graphics operations and VBE mode handling.
 *
 * This module abstracts the initialization of video modes and provides
 * functions to draw, flip and manage sprites on screen.
 */

/**
 * @brief Structure holding information about the current VBE video mode.
 *
 * Populated by `VG_init()` and used throughout the graphics system.
 */

vbe_mode_info_t mode_info;

int (VG_init)(uint16_t mode);
int (vg_map)(uint16_t mode);
void(VG_clear)();
void(swap_buffers)();
int(vg_draw_sprite_clipped)(sprite_t *sprite);
int(vg_draw_sprite)(sprite_t *sprite);
int(vg_draw_sprite_flipped)(sprite_t *sprite);



#endif
