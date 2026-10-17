#ifndef _VBE_MACROS_H_
#define _VBE_MACROS_H_

#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdbool.h>
#include <stdint.h>

static vbe_mode_info_t vbe_mode_info;
static mmap_t mem_map;
static void *video_mem;
static void *buff2;
static void *buff3;

/**
 * @brief Recieves mode and parses its info
 * 
 * @param mode mode to be parsed
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (get_mode_info)(uint16_t mode);

/**
 * @brief Initializes VRAM
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (map_vram)(void);

/**
 * @brief Obtains vram's physical address
 * 
 * @return Return 0 if success and 1 otherwise
 */
phys_bytes (get_phys_address)(void);

/**
 * @brief Obtains video mode width
 * 
 * @return Return width
 */
uint16_t (get_xres)(void);

/**
 * @brief Obtains video mode height
 * 
 * @return Returns height
 */
uint16_t (get_yres)(void);

/**
 * @brief Calculates bits per pixel
 * 
 * @return Returns bits per pixel
 */
uint16_t (get_bits_per_pixel)(void);

/**
 * @brief Calculates bytes per pixel
 * 
 * @return Returns bytes per pixel
 */
uint16_t (get_bytes_per_pixel)(void);

/**
 * @brief Calculates VRAM size
 * 
 * @return Returns VRAM size
 */
unsigned int (get_vram_size)(void);

/**
 * @brief Sets video card to a certain mode
 * 
 * @param mode mode desired
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (set_graphics_mode)(uint16_t mode);

/**
 * @brief Draws a pixel onto a buffer (buffer 1 and 3)
 * 
 * @param x x coordinte of the images's first pixel
 * @param y y coordinte of the images's first pixel
 * @param color color desired
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (vg_set_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a pixel onto a buffer (buffer 2)
 * 
 * @param x x coordinte of the images's first pixel
 * @param y y coordinte of the images's first pixel
 * @param color color desired
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (vg_set_pixel_b2)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a line in the screen
 * 
 * @param x x coordinte of the line's start
 * @param y y coordinte of the line's start
 * @param len line's lenght
 * @param color hex color code desired
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a line in the screen
 * 
 * @param x x coordinte of the rectangle's start
 * @param y y coordinte of the rectangle's start
 * @param width rectangle's width
 * @param height rectangle's height
 * @param color hex color code desired
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Clears screen
 * 
 * @return Return 0 if success and 1 otherwise
 */
int (clean_screen)();

/**
 * @brief Copies buffer 1 to buffer 3
 * 
 * @return void function
 */
void(buff1_to_3)();

/**
 * @brief Copies buffer 2 to buffer 1
 * 
 * @return void function
 */
void(buff2_to_1)();

/**
 * @brief Copies buffer 3 to buffer 2
 * 
 * @return void function
 */
void(buff3_to_2)();

/**
 * @brief Sets a xpm image onto a buffer (buffer 1 or 3)
 * 
 * @param xpm
 * @param x x coordinte of the images's first pixel
 * @param y y coordinte of the images's first pixel
 * 
 * @return void function
 */
void draw_xpm(xpm_map_t xpm, int x, int y);

/**
 * @brief Sets a xpm image onto a buffer (buffer 2)
 * 
 * @param xpm
 * @param x x coordinte of the images's first pixel
 * @param y y coordinte of the images's first pixel
 * 
 * @return void function
 */
void draw_cursor(xpm_map_t xpm, int x, int y);

#endif /* _VBE_MACROS_H_ */
