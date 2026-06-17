#ifndef VIDEO_GR_EXT_H
#define VIDEO_GR_EXT_H

#include "ninjix_platform.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t *pixels;
  uint16_t width;
  uint16_t height;
  uint8_t bytes_per_pixel;
} vg_sprite_t;

void *vg_init(uint16_t mode);
int vg_exit(void);

unsigned int get_h_res(void);
unsigned int get_v_res(void);
unsigned int get_bytes_per_pixel(void);
char *get_video_mem(void);
char *get_back_buffer(void);

int vg_prepare_buffer(void);
void vg_dispose_buffer(void);
size_t vg_get_frame_buffer_size(void);
int vg_clear_buffer(uint32_t color);
int vg_fill_rect_to_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int vg_present_buffer(void);

int vg_load_xpm_sprite(vg_sprite_t *sprite, xpm_map_t xpm);
void vg_destroy_sprite(vg_sprite_t *sprite);
int vg_capture_buffer_to_sprite(vg_sprite_t *sprite);
int vg_capture_region_to_sprite(vg_sprite_t *sprite, uint16_t x, uint16_t y);
int vg_draw_sprite_region_scaled_to_buffer(const vg_sprite_t *sprite, uint16_t src_x, uint16_t src_y,
                                           uint16_t width, uint16_t height, int16_t dst_x,
                                           int16_t dst_y, uint16_t scale);
int vg_draw_sprite_frame_scaled_to_buffer(const vg_sprite_t *sprite_sheet, uint16_t frame_width,
                                          uint16_t frame_height, uint16_t column, uint16_t row,
                                          int16_t dst_x, int16_t dst_y, uint16_t scale);
int vg_opaque_sprite_to_buffer(const vg_sprite_t *sprite, int16_t dst_x, int16_t dst_y);

#endif
