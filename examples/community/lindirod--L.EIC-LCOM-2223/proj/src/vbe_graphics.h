/**
 * @file vbe_graphics
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 * 
 *
 * @brief  This file contains the declarations of the functions used for the graphics card
 * 
 */
#ifndef _LCOM_VBE_GRAPHICS_H_
#define _LCOM_VBE_GRAPHICS_H_
#include <lcom/lcf.h>

#define BLACK 0x000000

#define YELLOW 0xffff00
#define PURPLE 0x800080
#define GREEN 0x00ff00
#define RED 0xff0000
#define BLUE 0x0000ff
#define ORANGE 0xff7f00

/**
 * @brief Draws a rectangle on the screen
 * 
 * @param x The x coordinate of the rectangle
 * @param y The y coordinate of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the rectangle
 * 
 * @return 0 on success, non-zero otherwise
 * 
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a vertical line on the screen
 * 
 * @param x The x coordinate of the line
 * @param y The y coordinate of the line
 * @param len The lenght of the line
 * @param color The color of the lines
 * 
 * @return 0 upon success, non-zero otherwise
 * 
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a horizontal line on the screen
 * 
 * @param x The x coordinate of the line
 * @param y The y coordinate of the line
 * @param len The lenght of the line
 * @param color The color of the line
 * @return 0 upon success, non-zero otherwise
 */
int (vg_draw_vline)(uint16_t x,uint16_t y,uint16_t len, uint32_t color);

/**
 * @brief Draws a pixel on the screen
 * 
 * @param x The x coordinate of the pixel
 * @param y The y coordinate of the pixel
 * @param color The color of the pixel
 * @return 0 upon success, non-zero otherwise
 * 
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws an xpm image on the screen
 * 
 * @param image The xpm image to draw
 * @param x The x coordinate of the image
 * @param y The y coordinate of the image
 * @return 0 upon success, non-zero otherwise
 */
int (draw_xpm)(xpm_image_t* image, uint16_t x, uint16_t y);

/**
 * @brief Erases an xpm image from the screen.
 * 
 * @param x The x coordinate of the image
 * @param y The y coordinate of the image
 * @param image The xpm image to erase
 * @return 0 upon success, non-zero otherwise
 */
void (erase_xpm)(uint16_t x, uint16_t y, xpm_image_t *img);

/**
 * @brief Gets the address of the video memory
 * 
 * @return char* - The address of the video memory 
 */
char * get_mem();

/**
 * @brief Gets the address of the buffer object
 * 
 * @return char* - The address of the buffer
 */
char * get_buffer();

/**
 * @brief Gets the size of the video memory
 * 
 * @return The size of the video memory
 */
unsigned get_vram_size();

/**
 * @brief Clears the screen
 * 
 */
void clean_screen();

/**
 * @brief Clears the buffer
 * 
 */
void clean_buffer();

/**
 * @brief Flips the buffer to the screen
 * 
 */
void video_flip();

#endif /*_LCOM_VBE_GRAPHICS_H_*/


