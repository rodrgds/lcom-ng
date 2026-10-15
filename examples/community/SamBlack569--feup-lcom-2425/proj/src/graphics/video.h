#ifndef __VIDEO_H
#define __VIDEO_H

#include <lcom/lcf.h>

int(vg_enter)(uint16_t mode);
int(vg_map_vram)(uint16_t mode);
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* buffer);
int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);
int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t* buffer);

void(load_buffer)();
void(load_menu_background)();
void(get_menu)();
void(get_game_background)();
void(clear_back_buffer)();
void(clear_double_buffer)();
void(clear_all_buffers)();
void(load_game_background)();


#endif // __VIDEO_H
