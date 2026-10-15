#ifndef _VIDEO_H
#define _VIDEO_H

#include <lcom/lcf.h>

int (video_set_mode)(uint16_t mode);
int (video_map_memory)(uint16_t mode);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

uint32_t (direct_color)(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first,  uint8_t step);
uint32_t (indexed_color)(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first,  uint8_t step);


#endif 
