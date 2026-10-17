#ifndef _LCOM_VIDEO_GR_H_
#define _LCOM_VIDEO_GR_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <minix/driver.h>
#include <sys/mman.h>

/**
 * @brief Initializes the video module in graphics mode
 * 
 */
void *(vg_init)(uint16_t mode);
/**
 * @brief Draw pixel with specified color at position with x and y coordinates
 */
int set_pixel(uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief Draws a horizontal line by changing the video RAM
 * 
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief Draws a filled rectangle by changing the video RAM
 * 
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
 * @brief Function used to draw an image inthe screen
 * 
 * @param img 
 * @param x 
 * @param y 
 * @return int 
 */
int vg_draw_img(xpm_image_t img, uint16_t x, uint16_t y);

/**
 * @brief Updates mouse's coordinates
 * 
 * @param deltaX 
 * @param deltaY 
 */
void update_mouse_position(int deltaX, int deltaY);
/**
 * @brief Processes graphics data in one buffer while the next set of graphics data is read into the other buffer
 * 
 */
void doubleBuffering();
#endif /* _LCOM_VIDEO_GR_H */
