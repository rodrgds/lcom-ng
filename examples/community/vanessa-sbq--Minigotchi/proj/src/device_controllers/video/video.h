#include <lcom/lcf.h>
#include "video_macros.h"

#ifndef __VIDEO_H
#define __VIDEO_H

void returnToTextMode();
int video_get_mode_information(uint16_t mode);
int video_map_vram();
int video_set_graphics_mode(uint16_t mode);
unsigned int video_get_phys_addr();
unsigned int video_get_vram_size();
unsigned getBytesPerPixel();

uint16_t getXResolution();

void getBufferFromSprite(uint16_t height, uint16_t width, uint16_t x, uint16_t y, uint32_t *colors, char** background_buffer);
int setBackgroundFromBuffer(char* background_buffer);

int drawPixel(uint16_t x, uint16_t y, uint32_t color);
int vg_draw_pixmap(uint16_t x, uint16_t y, uint8_t* map, xpm_image_t pixmap_and_metadata);
void vg_clear_screen();
int vg_page_flip();

#endif /* __VIDEO_H */
