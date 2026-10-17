#pragma once
#include <stdint.h>

typedef uint8_t u_char;

#define BIT(n) (0x01<<(n))

#define INT 0x10

#define TEXT_MODE 0x03

#define INFORMATION_VBE_CONTROLLER 0x00
#define INFORMATION_VBE_MODE 0x01
#define SET_VBE_MODE 0x02

#define VBE_MODE_1024x768 0x105
#define VBE_MODE_640x480 0x110
#define VBE_MODE_800x600 0x115
#define VBE_MODE_1280x1024 0x11A
#define VBE_MODE_1152x864 0x14C

#define VBE_SET_LINEAR_MODE BIT(14)

#define WHITE 0xFFFF
#define BLACK 0x0001
#define YELLOW 0xDE86
#define GREEN 0x07e0
#define RED 0xf800

int h_res;
int v_res;
int bits_pix;
char *v_mem;
u_char *aux_v_mem;

int getVerResolution();

int getHorResolution();

u_char* getGraphicsBuffer();

uint32_t convert_phys_to_virtual(uint32_t p);

void flip_buff();

int vg_change_mode(uint16_t mode);

void *(vg_init)(uint16_t mode);

int vg_color_pixel(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len , uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

void vg_draw_sprite(uint16_t* m_sprite, uint16_t h_sprite, uint16_t w_sprite, uint16_t y, uint16_t x);

void vg_draw_text_sprite(char* m_sprite, uint16_t h_sprite, uint16_t w_sprite,
  uint16_t h_partial, uint16_t w_partial, uint16_t begin_y, uint16_t begin_x, uint16_t y, uint16_t x, uint32_t color, int text_size);

void vg_clear();

uint8_t RGB_first(uint8_t maskSize, int8_t Position, uint32_t color);

uint32_t d_color(uint16_t i, uint16_t j, uint32_t first, uint8_t step);
