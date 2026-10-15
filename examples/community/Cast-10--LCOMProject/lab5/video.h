#ifndef VIDEO_H
#define VIDEO_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "vbe.h"

int vbe_set_mode(uint16_t mode);

int(set_vram_mapping)(uint16_t mode);

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int(vg_draw_rectangle)(uint16_t x, uint16_t y,
                       uint16_t width, uint16_t height,
                       uint32_t color);

int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

int(vg_draw_pixmap)(uint8_t *pixmap,uint16_t width,uint16_t height,int x,int y,uint32_t transparent_color);


int video_init_page_flipping(uint16_t mode);

int video_swap_buffers(void);

int video_clear_back_buffer(const uint8_t *background);

#endif

