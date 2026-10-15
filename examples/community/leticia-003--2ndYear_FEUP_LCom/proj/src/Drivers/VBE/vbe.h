#ifndef _VBE_H_
#define _VBE_H_

#include <lcom/lcf.h> 
extern void *video_mem;
extern phys_bytes vram_base;
extern size_t vram_size;
extern uint16_t h_res;
extern uint16_t v_res;
extern uint16_t bits_per_pixel;

int set_graphics_mode(uint16_t mode);

void sleep_for_seconds(uint8_t delay);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

int map_vram(uint16_t mode);

int vg_draw_xpm(int16_t x, int16_t y, uint16_t width,  uint16_t height, uint8_t *bytes,  bool square_shape);

void vg_clear_screen();

void draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step, const vbe_mode_info_t *mode_info);

void vg_disable_double_buffering(void);

int vg_enable_double_buffering(void);

void vg_swap_buffers(void);

#endif 
