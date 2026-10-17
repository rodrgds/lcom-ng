#include "rectangle.h"
#include "labyrinth.h"
#include "video_gr.h"
#include "mouse.h"

Labyrinth* newLabyrinth(){
	Labyrinth* lab = (Labyrinth*) malloc(sizeof(Labyrinth));
	lab->rec1 = newRectangle(80, 300, 250, 60);
	lab->rec2 = newRectangle(280, 300, 50, 240);
	lab->rec3 = newRectangle(280, 500, 230, 40);
	lab->rec4 = newRectangle(480, 100, 30, 430);
	lab->rec5 = newRectangle(480, 100, 220, 20);
	lab->rec6 = newRectangle(680, 100, 20, 200);
	lab->finish = newRectangle(680, 280, 20, 20);
	return lab;
}

void drawLabyrinth(Labyrinth* lab) {
	drawRectangle(lab->rec1, rgb(0, 255, 200), getGraphicsBuffer());
	drawRectangle(lab->rec2, rgb(0, 255, 200), getGraphicsBuffer());
	drawRectangle(lab->rec3, rgb(0, 255, 200), getGraphicsBuffer());
	drawRectangle(lab->rec4, rgb(0, 255, 200), getGraphicsBuffer());
	drawRectangle(lab->rec5, rgb(0, 255, 200), getGraphicsBuffer());
	drawRectangle(lab->rec6, rgb(0, 255, 200), getGraphicsBuffer());
	drawRectangle(lab->finish, rgb(255, 0, 0), getGraphicsBuffer());
}

int insideLab(Labyrinth* lab, Rectangle* rect){
	if (!rectangleInside(rect, lab->rec1) && !rectangleInside(rect, lab->rec2) && !rectangleInside(rect, lab->rec3) && !rectangleInside(rect, lab->rec4) && !rectangleInside(rect, lab->rec5) && !rectangleInside(rect, lab->rec6))
		return 1;
	else
		return 0;
}

void deleteLabyrinth(Labyrinth* lab) {
	free(lab);
}
