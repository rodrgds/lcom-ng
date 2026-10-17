#ifndef _SRC_GRAPHICS_H_
#define _SRC_GRAPHICS_H_

#include "VBE.h"
#include <lcom/lcf.h>

vbe_mode_info_t mode_info;

int(set_graphic_mode)(uint16_t mode);
int(set_fb)(uint16_t mode, uint8_t **fb);
int(draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *fb);
int(draw_horizontal_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *fb);
int(draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *fb);

#endif
