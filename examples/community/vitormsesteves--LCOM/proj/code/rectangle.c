#include "rectangle.h"
#include "video_gr.h"

Rectangle* newRectangle(int x, int y, int w, int h) {
	Rectangle * rectangle = (Rectangle*) malloc(sizeof(Rectangle));
	rectangle->x = x;
	rectangle->y = y;
	rectangle->w = w;
	rectangle->h = h;
	return rectangle;
}

void deleteRectangle(Rectangle * rectangle) {
	free(rectangle);
}

void setRecLocation(Rectangle * rectangle, int x, int y) {
	rectangle->x = x;
	rectangle->y = y;
}

void moveRectangle(Rectangle* rectangle, int x, int y) {
	rectangle->x += x;
	rectangle->y += y;
}

void drawRectangle(Rectangle* rectangle, int color, char* buf) {
	draw_rectangle(rectangle->x, rectangle->y, rectangle->x+rectangle->w,
			rectangle->y+rectangle->h, color, buf);
}

int rectanglesColliding(Rectangle* r1, Rectangle* r2) {
	return !(r1->x + r1->w < r2->x || r1->x > r2->x + r2->w || r1->y + r1->h < r2->y  || r1->y > r2->y + r2->h);
}

int rectangleInside(Rectangle* r1, Rectangle* r2){
	return (r2->x <= r1->x && r1->x+r1->w <= r2->x+r2->w && r2->y <= r1->y && r1->y+r1->h <= r2->y+r2->h);
}
