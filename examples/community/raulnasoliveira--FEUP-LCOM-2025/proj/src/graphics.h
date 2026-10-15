#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>

#include "VBE.h"
#include "font8x8_basic.h"

extern uint8_t *double_buffer;     
extern uint8_t *frame_buffer;      
extern unsigned int frame_size;    


vbe_mode_info_t information;
uint8_t* frame_buffer;

int (graphics_mode)(uint16_t sub);

int (set_frame_buffer)(uint16_t mode);

int (vg_draw_pixel)(int x, int y, uint32_t color);

int (vg_draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t* buffer);

int (my_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t* buffer);

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B);

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);

uint32_t (Red)(unsigned j, uint8_t step, uint32_t first);

uint32_t (Green)(unsigned i, uint8_t step, uint32_t first);

uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first);

uint32_t (R)(uint32_t first);

uint32_t (G)(uint32_t first);

uint32_t (B)(uint32_t first);

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t *buffer);

void draw_text_centered(const char* text, int y, uint32_t color, uint8_t* buffer);

int vg_draw_char(char c, uint16_t x, uint16_t y, uint32_t color, uint8_t* buffer);

int vg_draw_pixel_buffer(uint8_t *buffer, int x, int y, uint32_t color);

void print_xpm_string_centered(const char *str, int y, uint8_t *buffer, int screen_width);

const xpm_row_t* get_alphabet_xpm_highlight(char c, int highlight);

const xpm_row_t* get_alphabet_xpm(char c);

void print_xpm_string_centered_highlight(const char *str, int y, uint8_t *buffer, int screen_width, int highlight);

#endif 
