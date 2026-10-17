#ifndef CURSOR_H
#define CURSOR_H

#include "xpm.h"
#include "video.h"
#include "pixmap/cursor_pixmap.h"

int startCursor();

void getPosition(int *x, int *y);

void updatePosition(struct packet *pp, int *x, int *y);

void clearMouse();

void underMouse();


#endif
