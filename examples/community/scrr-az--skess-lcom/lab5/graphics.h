#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include "VBE.h"
#include <lcom/lcf.h>

void* (vg_init)(uint16_t mode);
void* (set_frame_buffer)(uint16_t mode, int* one_buffer_size);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_color_mode)(uint32_t* color);
int (vg_draw_xpm)(xpm_image_t img, uint8_t* map, uint16_t x, uint16_t y);
int (change_display_buffer)();

#endif /* _LCOM_GRAPHICS_H_ */
