#include <lcom/lcf.h>
#pragma once
#include "game_structs.h"

extern unsigned int x_res;
extern unsigned y_res;

extern unsigned color_mode;
extern unsigned int bppixel;
extern unsigned int bytes_per_pixel;
static unsigned int vram_base;  
static unsigned int vram_size; 
vbe_mode_info_t mode_info;
struct reg86 r;  
static void* v_mem;


/**
 * @brief Function implemented in lab5.
 * 
 */
int vbe_get_mode_inf(uint16_t mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Function implemented in lab5.
 * 
 */
int set_vbe_mode(uint16_t mode);

/**
 * @brief Function implemented in lab5.
 * 
 */
void*(vg_init)( uint16_t mode) ;	

/**
 * @brief Function implemented in lab5.
 * 
 */
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Function implemented in lab5.
 * 
 */
int (vg_draw_rectangle)(uint16_t	x, uint16_t  y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Function implemented in lab5.
 * 
 */
int (vg_draw_hline)(uint16_t	x, uint16_t  y, uint16_t len, uint32_t color);

/**
 * @brief Function implemented in lab5.
 * 
 */
int get_mode_info(uint16_t mode);

/**
 * @brief Function implemented in lab5.
 * 
 */
int draw_xpm(int x ,int y, xpm_map_t img);

/**
 * @brief Function implemented in lab5.
 * 
 */
int draw_sprite(int x ,int y, xpm_image_t *img);

/**
 * @brief Function implemented in lab5.
 * 
 */
int clear_screen();

/**
 * @brief Function implemented in lab5.
 * 
 */
int draw_xpm_floor(int x ,int y, xpm_map_t xpm);

/**
 * @brief Function implemented in lab5.
 * 
 */
int draw_background(Background *b);

/**
 * @brief Function implemented in lab5.
 * 
 */
int refresh_graphics();

