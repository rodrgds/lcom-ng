#include "rectangle.h"
#include "graphics.h"

#define GREEN  0xE007

Rectangle* newRectangle(int xi,int yi, int xf,int yf){

	Rectangle* rect =(Rectangle*) malloc(sizeof(Rectangle));

	rect->xi = xi;
	rect->xf = xf;
	rect->yi = yi;
	rect->yf = yf;

	return rect;
}


void deleteRectangle(Rectangle* rect){
	free(rect);
}

void drawRectangle(Rectangle* rect){
	int i;

	unsigned short xi; xi = rect->xi*2;
	unsigned short xf; xf = rect->xf*2;
	unsigned short yi; yi = rect->yi*2;
	unsigned short yf; yf = rect->yf*2;

	unsigned short color; color = GREEN;
	unsigned short width; width = xf -xi;
	unsigned short height; height = yf - yi;

	for(i = xi; i < xf; i++){
		set_pixel(i, yi, color);
		set_pixel(i, yf, color);
	}
}
