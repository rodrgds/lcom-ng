#ifndef _VG_H_
#define _VG_H_
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>

typedef struct 
{
    int x;
    int y;
} Pixel;


uint8_t get_red_field_position(void);

uint8_t get_green_field_position(void);

uint8_t get_blue_field_position(void);

uint16_t get_red_mask_size(void);

uint16_t get_green_mask_size(void);

uint16_t get_blue_mask_size(void);

uint16_t get_horizontal_resolution(void);

uint16_t get_vertical_resolution(void);

unsigned get_bits_per_pixel(void);

int free_map_memory(void);

int (draw_sprite)(uint8_t *sprite, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

int (draw_element)(xpm_row_t *pixmap, uint16_t x, uint16_t y);

void *(vg_init)(uint16_t mode);

uint8_t *(read_pixmap)(xpm_row_t *pixmap, uint16_t *width, uint16_t *height);

int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len,uint32_t color);

int draw_pixel(uint16_t x, uint16_t y, uint32_t color);

int draw_element_scaled(xpm_row_t *pixmap, uint16_t x, uint16_t y, float scale);

void vg_draw_content(); 

int clearScreen();

//TEMP
int draw_element_with_color(xpm_row_t *pixmap, uint16_t x, uint16_t y, uint32_t color);






#endif /* _VG_H_ */
