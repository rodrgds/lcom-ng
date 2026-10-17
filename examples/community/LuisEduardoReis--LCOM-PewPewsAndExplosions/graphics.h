#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "video_gr.h"

typedef struct {
	unsigned char r,g,b;
} rgbPixel;


typedef struct {
	int width, height;
	int offsetX, offsetY;
	unsigned char* pixels;
} rgbImage;

typedef struct {
	int width, height;
	unsigned char* pixels;
} image;


int loadBMP(char* filename,rgbImage* res);

int drawImage(rgbImage img, int x, int y);

int drawImageBlend(rgbImage img, int x, int y, unsigned int blend);

rgbImage cropImage(rgbImage img, int x, int y, int w, int h);

rgbImage cropImageCentered(rgbImage img, int x, int y, int w, int h);


unsigned int hsvTorgb(unsigned int h, float s, float v);

#endif
