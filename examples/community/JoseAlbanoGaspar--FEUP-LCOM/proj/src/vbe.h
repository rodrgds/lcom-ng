#ifndef __VBE_H
#define __VBE_H

#include <lcom/lcf.h>
#include "macros/constants.h"
#include "sprite.h"

int r;
struct minix_mem_range mr; /* Physical memory range */
unsigned int vram_base;    /* VRAM's physical addresss */
unsigned int vram_size;    /* VRAM's size, but you can use the frame-buffer size, instead */
static void *video_mem;        /* Frame-buffer VM address */
uint16_t h_res;            /* Horizontal resolution in pixels */
uint16_t v_res;            /* Vertical resolution in pixels */
uint16_t bitsPerPixel;
uint16_t bytesPerPixel;
uint8_t red_s;
uint8_t red_p;
uint8_t green_s;
uint8_t green_p;
uint8_t blue_s;
uint8_t blue_p;
uint8_t memoryModel;
phys_bytes phys_addr;
static void *double_buffer;

/**
 * @brief pixmap to be displayed on the screen
 */
enum pixmap {SCORE, TITLE, CROSSHAIR, ENEMY, DEAD_ENEMY, APPLE, NUMBER0, NUMBER1, NUMBER2, NUMBER3, NUMBER4, NUMBER5,
              NUMBER6, NUMBER7, NUMBER8, NUMBER9, GAMEOVER, EXIT, PLAY, GO_BACK_1, GO_BACK_2,TWO_POINTS,SLASH};


/**
 * @brief gets the bitmask
 * @param k byte to get bitmask
 * @return the bitmask of the parameter byte
 */
uint32_t getmask(int k);
/**
 * @brief Initializes the video module in graphics mode
 * @param mode 16-bit VBE mode to set
 */
void *(vg_init)(uint16_t mode);
/**
 * @brief Returns information on the input VBE mode
 * @param mode mode whose information should be returned
 * @param vmi_p address of vbe_mode_info_t structure to be initialized
 * @return returns 0 on success, non-zero otherwise
 */
int (vg_get_mode_info)(uint16_t mode, vbe_mode_info_t * vmi_p);
/**
 * @brief Draws only a pixel by changing the video RAM
 * @param x horizontal coordinate of the point
 * @param y vertical coordinate of the point
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief Draws a verical line by changing the video RAM
 * @param x horizontal coordinate of the initial point
 * @param y vertical coordinate of the initial point
 * @param len line's length in pixels
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 */
int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief Draws a horizontal line by changing the video RAM
 * @param x horizontal coordinate of the initial point
 * @param y vertical coordinate of the initial point
 * @param len line's length in pixels
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief Draws a filled rectangle by changing the video RAM
 * @param x horizontal coordinate of the rectangle's top-left vertex
 * @param y vertical coordinate of the rectangle's top-left vertex
 * @param width rectangle's width in pixels
 * @param height rectangle's height in pixels
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
 * @brief Draws a rectangle-based colored pattern
 * @param no_rectangles Number of rectangles in each of the horizontal and vertical direction
 * @param first Color to be used in the first rectangle
 * @param step Difference between the values of the R/G component in adjacent rectangles
 * @return 0 on success, non-zero otherwise
 */
int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);
/**
 * @brief generats pixmaps dynamically
 * @param x horizontal coordinate
 * @param y vertical coordinate
 * @param mode current video mode
 * @param pixtype type of the pixmap
 * @return 0 on success, non-zero otherwise
 */
int (vg_ultimate_pixmap_handler)(uint16_t x, uint16_t y,uint16_t mode, enum pixmap pixtype);
/**
 * @brief erases pixmaps dynamically
 * @param x horizontal coordinate
 * @param y vertical coordinate
 * @param mode current video mode
 * @param pixtype type of the pixmap
 * @param mouse
 * @return 0 on success, non-zero otherwise
 */
int (vg_ultimate_pixmap_eraser)(uint16_t x, uint16_t y,uint16_t mode, enum pixmap pixtype, bool mouse);
/**
 * @brief draws the pixmax on the screen
 * @param xpm map (assuming indexed color mode)
 * @param x horizontal coordinate of upper-left corner, starts at 0 (leftmost pixel)
 * @param y	vertical coordinate of upper-left corner, starts at 0 (top pixel)
 * @return returns 0 on success, non-zero otherwise
 */
int(vg_draw_pixmap)(xpm_map_t xpm, uint16_t x, uint16_t y);
/**
 * @brief erases the pixmax of the screen
 * @param xpm map (assuming indexed color mode)
 * @param x horizontal coordinate of upper-left corner, starts at 0 (leftmost pixel)
 * @param y	vertical coordinate of upper-left corner, starts at 0 (top pixel)
 */
void(vg_erase_pixmap)(xpm_map_t xpm, uint16_t x, uint16_t y);
/**
 * @brief creates the sprite to be displayed
 * @param xpm map (assuming indexed color mode)
 * @param img pointer to image information
 * @return returns the sprite created
 */
uint8_t *(vg_create_sprite)(xpm_map_t xpm, xpm_image_t *img);
/**
 * @brief draws the sprite on the screen
 * @param *sprite pointer to where is stored the sprite
 * @param img map (assuming indexed color mode)
 * @param x horizontal coordinate of upper-left corner, starts at 0 (leftmost pixel)
 * @param y	vertical coordinate of upper-left corner, starts at 0 (top pixel)
 * @return returns 0 on success, non-zero otherwise
 */
int(vg_draw_sprite)(uint8_t *sprite, xpm_image_t img, uint16_t x, uint16_t y);
/**
 * @brief erases the sprite on the screen
 * @param *sprite pointer to where is stored the sprite
 * @param img map (assuming indexed color mode)
 * @param x horizontal coordinate of upper-left corner, starts at 0 (leftmost pixel)
 * @param y	vertical coordinate of upper-left corner, starts at 0 (top pixel)
 * @return returns 0 on success, non-zero otherwise
 */
int(vg_erase_sprite)(uint8_t *sprite, xpm_image_t img, uint16_t x, uint16_t y);
/**
 * @brief swaps current buffer
 */
void (swapBuffer)();


#endif /* __VBE_H */
