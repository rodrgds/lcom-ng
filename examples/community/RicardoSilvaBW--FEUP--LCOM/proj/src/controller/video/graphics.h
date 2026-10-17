#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int set_video_mode(uint16_t mode);

int vram_start(uint16_t mode);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
unsigned int get_bytes_per_pixel();

uint16_t get_h_res();
uint16_t get_v_res();
void clear_buffer();
void swap_buffer();
int draw_xpm(xpm_image_t img, uint8_t *colors, int x, int y);
int draw_xpm_scaled(xpm_image_t img, uint8_t *colors, int x, int y, int scale);

void (set_static_background)();
void (provide_static_background)();
void (delete_static_background)();

uint8_t* (get_back_buffer)();
unsigned int (get_vram_size)();

#endif
