#ifndef __VIDEO_H
#define __VIDEO_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>

struct masks
{
   uint32_t red;
   uint32_t green;
   uint32_t blue; 
};

struct rgb_8_8_8_t{
uint8_t comp[3];
};
int (set_graphic_mode)(uint16_t mode);
int (allocVirtMem)(uint16_t mode);
void(reset_video)();
int (createMasks)();
int (colorCorrection)(uint32_t *color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (paint)(uint16_t x,uint16_t y,uint32_t color);
uint32_t (getRed(uint32_t color));
uint32_t (getGreen(uint32_t color));
uint32_t (getBlue(uint32_t color));
void(swapBuff)();
int(show_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);
#endif
