#ifndef XPM_H
#define XPM_H

//#pragma once
#include "video.h"

void draw_xpm(int x, int y, uint16_t *sprite, xpm_image_t img, const uint16_t transp);

uint16_t getTransparency();

int align_x_center(xpm_image_t img);

#endif
