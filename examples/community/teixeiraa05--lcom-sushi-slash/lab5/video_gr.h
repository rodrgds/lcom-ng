#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <stdint.h>
#include "vesa-bios.h"

int (set_video_mode)(uint16_t mode);
void* (graphics_init)(uint16_t mode);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y, enum xpm_image_type type);
int (vg_exit)(void);
int (vg_display_vbe_contr_info)(struct vg_vbe_contr_info *info_p);

/* Page-flipping (double-buffer) functions */

int video_init_page_flipping(uint16_t mode);
int video_clear_page(void);
int video_swap_pages(void);
int video_cleanup_page_flipping(void);
int video_draw_background(const uint8_t *background_pixmap);
int video_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
int video_draw_sprite(const uint8_t *pixmap, uint16_t width, uint16_t height, uint16_t x, uint16_t y);
int video_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

#endif /* __VIDEO_GR_H */
