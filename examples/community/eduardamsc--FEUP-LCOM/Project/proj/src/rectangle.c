#include "rectangle.h"

#include <minix/drivers.h>
#include <minix/driver.h>

Rectangle* newRectangle(int xi, int yi, int xf, int yf){
	Rectangle* rect = (Rectangle*) malloc(sizeof(Rectangle));

	rect->xi=xi;
	rect->yi=yi;
	rect->xf=xf;
	rect->yf=yf;

	return rect;
}

void deleteRectangle(Rectangle* rect){
	free(rect);
}
