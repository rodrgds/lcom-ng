#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>

extern vbe_mode_info_t mode_info; 

int set_graphics_mode(uint16_t mode);
int map_vram(uint16_t mode);
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int print_xpm(const char *xpm[], uint16_t x, uint16_t y);

#endif /* GRAPHICS_H */
