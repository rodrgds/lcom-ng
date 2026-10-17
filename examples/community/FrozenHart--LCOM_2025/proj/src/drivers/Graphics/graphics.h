#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "sprite.h"
#include <lcom/lcf.h>
#include <machine/int86.h> // /usr/src/include/arch/i386

int(set_graphic_mode)(uint16_t mode);
int(set_text_mode)(); // vg_exit()
int(map_VRAM)(unsigned short mode);
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int(get_color)(uint32_t old_color, uint32_t *new_color, uint16_t row, uint16_t col, uint16_t no_rectangles, uint16_t step);
uint16_t(get_v_resolution)();
uint16_t(get_h_resolution)();
int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);
int(vg_draw_xpm)(xpm_image_t img, uint8_t *colors, uint16_t x, uint16_t y);
uint8_t *(vg_load_xpm) (xpm_map_t xpm, xpm_image_t *img);
int(vg_erase_xpm)(xpm_image_t img, uint16_t x, uint16_t y);
int vg_read_pixel(uint16_t x, uint16_t y, uint32_t *color);
#endif // GRAPHICS_H
