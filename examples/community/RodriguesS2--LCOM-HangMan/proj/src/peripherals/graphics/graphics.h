#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "VBE.h"
#include <lcom/lcf.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

typedef struct {
  uint16_t width;  // largura do retângulo
  uint16_t height; // altura do retângulo
} RectangleSize;

typedef struct {
  uint8_t r, g, b; // auxiliar para guardar o RGB
} RGBColor;

extern uint8_t *main_buffer;
extern uint8_t *back_buffer;
extern uint8_t bytes_per_pixel;

int init_double_buffer();
void clear_back_buffer();
void swap_buffers();
void free_double_buffer();

int(end_loop_ESC)();

int(vg_init_graphic)(uint16_t mode);

int map_main_buffer(uint16_t mode);

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int normalize_color(uint32_t color, uint32_t *new_color);

int calculate_size(RectangleSize *rectangle, uint8_t no_rect, uint16_t x, uint16_t y);

uint32_t pack_rgb(RGBColor *rgb);

int draw_pattern(RectangleSize *rectangle, uint8_t no_rect, uint32_t first, uint8_t step);

uint32_t R(uint32_t color);

uint32_t G(uint32_t color);

uint32_t B(uint32_t color);

int draw_xpm(uint8_t *pixmap, xpm_image_t *img, uint16_t x, uint16_t y);

int calculate_move_amount(int speed, int target_pos, int current_pos);

void graphics_exit(void);

void clear_screen();

uint32_t vg_get_pixel(uint16_t x, uint16_t y);

#endif
