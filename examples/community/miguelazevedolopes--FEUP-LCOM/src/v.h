#include <stdint.h>
#include <stdio.h>
/** @defgroup v v.h
 * @{
 *
 * This file contains functions responsible for the video card working.
 * 
 */

void*(vg_init)(uint16_t mode) ;
void*(map_vm)(uint16_t mode);
void (vg_colour_pixel)(uint16_t x,  uint16_t y, uint32_t color);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
void (vg_draw_xpm)(uint8_t *pixmap, xpm_image_t img, uint16_t x, uint16_t y);
bool (is_vertical_movement)(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf);
int (direction)(uint16_t si, uint16_t sf);
void (move_xmp)(uint16_t *xi, uint16_t *yi, uint16_t xf, uint16_t yf, uint16_t vx, uint16_t vy, uint16_t speed, uint8_t *pixmap, xpm_image_t img);
void (flip_buffers)();
/**@}*/
