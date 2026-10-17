#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "sprite.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */
char *double_buffer;

char *video_mem;

unsigned video_mem_size;

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

unsigned getHRES();

//void drawBitmap(Bitmap* bitmap, int x, int y, Alignment alignment);

unsigned getVRES();

unsigned getBitsPerPixel();

void colorPixel(int x, int y, char * video_mem, short color);

void drawSlopingLine(int xi, int yi, int xf, int yf, short color);

void drawSprite(Sprite *s);

void drawLine(int xi, int yi, int xf, int yf, char * video_mem, short color);

char * getVideoMem();

void changeVideoMem();

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
