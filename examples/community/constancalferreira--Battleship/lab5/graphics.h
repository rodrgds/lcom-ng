#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <machine/int86.h>
#include "../lab3/kbd.h"


//kbd related
int (wait_for_ESC)();

//graphics
int (set_vbe_mode)(uint16_t mode);
int (set_frame_buffer)(uint16_t mode);

int (normalize_color)(uint32_t color, uint32_t *new_color);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int(print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);


#endif /* GRAPHICS_H */
