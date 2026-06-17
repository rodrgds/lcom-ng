#ifndef PVZ_VIDEO_H
#define PVZ_VIDEO_H

#include "pvz_platform.h"

#include <stdint.h>

void *vg_init(uint16_t mode);
int vg_exit(void);
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
int vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int vg_draw_xpm(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y);
void vg_flush(void);
void vg_cleanup(void);
void vg_copy_to_buffer(uint8_t *src);
int vg_draw_pixmap(uint8_t *pixmap, uint16_t width, uint16_t height, int x, int y);
uint16_t get_h_res(void);
uint16_t get_v_res(void);
uint8_t get_bits_per_pixel(void);

#endif
