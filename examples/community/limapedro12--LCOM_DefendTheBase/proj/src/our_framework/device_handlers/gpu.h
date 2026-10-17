#ifndef GPU_H
#define GPU_H

#include <lcom/lcf.h>
uint NO_BACKGROUND;
unsigned char* double_buffer;

/**
 * @brief initializes the video card in the given mode
 * 
 * @param mode unsigned 16 bit integer that represents the mode to be initialized
 */
void* (vg_init)(u16_t mode);

/**
 * @brief function that draws a pixel in the given coordinates with the given color
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param color color of the pixel
 */
void draw_pixel(int x, int y, uint color);

/**
 * @brief function that draws a rectangle in the given coordinates with the given color
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color color of the rectangle
 * @param mode mode of the rectangle
 */
void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint16_t mode);

/**
 * @brief Get the width of the screen corresponding to the given mode
 * 
 * @param mode unsigned 16 bit integer that represents the mode
 * @return int width of the screen
 */
int get_width(uint16_t mode);

/**
 * @brief Get the height of the screen corresponding to the given mode
 * 
 * @param mode unsigned 16 bit integer that represents the mode
 * @return int height of the screen
 */
int get_height(uint16_t mode);

/**
 * @brief function that turns off video graphics and returns to text mode
 * 
 * @return int equal to 0 if the function was successful, 1 otherwise
 */
int gpu_exit();

/**
 * @brief places the double buffer in the VRAM
 * 
 * @return int equal to 0 if the function was successful, 1 otherwise
 */
int doublebuffer_to_vram();

/**
 * @brief function that draws a xpm in the given coordinates
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param xpm xpm to be drawn
 * @param color_to_ignore color to be ignored
 * @return int equal to 0 if the function was successful, 1 otherwise
 */
int draw_xpm(uint16_t x, uint16_t y, xpm_map_t xpm, uint color_to_ignore);

/**
 * @brief draws a xpm after it being loaded by the function xpm_load of lcf
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param xpm xpm to be drawn
 * @param img xpm image
 * @param color_to_ignore color to be ignored
 * @return int equal to 0 if the function was successful, 1 otherwise
 */
int draw_xpm_loaded(uint16_t x, uint16_t y, uint8_t* xpm, xpm_image_t img, uint color_to_ignore);

/**
 * @brief Get the size of the VRAM
 * 
 * @return int equal to the size of the VRAM
 */
int get_vram_size();
#endif
