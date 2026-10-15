#ifndef GPU_H
#define GPU_H

#include <lcom/video_gr.h>

int vg_set_mode(uint16_t mode);
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);
int clear_screen();
void draw();
void freebuffer();
void draw_grid();
uint16_t get_hres();
uint16_t get_vres();

#endif
