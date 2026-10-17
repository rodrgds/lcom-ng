#include <minix/drivers.h>
#include "Button.h"

Box* newBox(int x1, int y1, int x2, int y2){
	Box* box=(Box*)malloc(sizeof(Box));

	box->x1=x1;
	box->y1=y1;
	box->x2=x2;
	box->y2=y2;

	return box;
}

void deleteBox(Box* box){
	free(box);
}
