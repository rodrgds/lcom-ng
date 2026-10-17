#ifndef __V_CARD_H
#define __V_CARD_H

#include <lcom/lcf.h>

int(change_mode)(uint16_t mode);
void *(vg_init)(uint16_t mode);
//int  (vg_exit)();

// Call once at the end of each frame to push the back buffer to the screen
void (vg_flip)();

// Fill the entire back buffer with a color before drawing the next frame
void (vg_clear)(uint32_t color);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_xpm)(xpm_image_t img, uint8_t *pixmap, uint16_t x, uint16_t y);

#endif /* __V_CARD_H */
