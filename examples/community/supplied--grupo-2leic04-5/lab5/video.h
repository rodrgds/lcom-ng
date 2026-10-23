#ifndef GRUPO_2LEIC04_5_VIDEO_H
#define GRUPO_2LEIC04_5_VIDEO_H
#include <stdint.h>
int my_map_vram(uint16_t mode);
int my_vg_init(uint16_t mode);
int my_vg_enable_double_buffer(void);
int my_vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
int my_vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int my_vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void my_vg_flip_buffer(void);
void my_vg_free_double_buffer(void);
#endif
