#pragma once

#include "vbe.h"
#include <lcom/lcf.h>

int(gpu_set_mode)(uint16_t mode);

int(gpu_map_vram)(uint16_t mode);

void(swap_buffers)();

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

int (paint_pixel)(uint16_t x, uint16_t y, uint32_t color);
