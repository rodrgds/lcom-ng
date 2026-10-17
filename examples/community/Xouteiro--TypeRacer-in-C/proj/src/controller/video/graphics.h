#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"

vbe_mode_info_t mode_info;

int (graphics_set_mode)(uint16_t submode);
int (graphics_set_frame_buffer)(uint16_t mode, uint8_t** frame_buffer);
int (graphics_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer);
int (graphics_draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t* frame_buffer);
int (graphics_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t* frame_buffer);

#endif
