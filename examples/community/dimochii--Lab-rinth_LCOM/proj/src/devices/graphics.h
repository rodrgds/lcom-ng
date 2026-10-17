#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Frame {
    uint8_t *buffer1, *buffer2;
    unsigned int res_x, res_y, size, bytes_pixel;
} Frame; 

Frame frame;
vbe_mode_info_t mode_info;
int display_buffer; uint8_t *draw_buffer; uint8_t *bg_buffer;

int (graphic_mode)(uint16_t mode);
int (initialize_frame_buffer) (uint16_t mode);

void (set_display_start)(int buffer);
void (update_flip_frames)();
void (clear_drawing)();

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* buffer);
int (draw_background)();

void (initialize_buffers)();
void (free_buffers)();

#endif
