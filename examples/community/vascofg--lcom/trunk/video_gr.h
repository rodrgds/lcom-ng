#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

void *vg_init(unsigned short mode);

int vg_fill(unsigned long color);

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color);

long vg_get_pixel(unsigned long x, unsigned long y);

int vg_draw_line(unsigned long xi, unsigned long yi, 
		           unsigned long xf, unsigned long yf, unsigned long color);

void vg_exit(void);

void printChar(unsigned long x, unsigned long y, char ch, unsigned long fg, unsigned long bg);
 
#endif /* __VIDEO_GR_H */
