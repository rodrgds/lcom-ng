#include "Collisions.h"

#define OBS_LEFT_X   200
#define OBS_CENTER_X 340
#define OBS_RIGHT_X  480
#define POKE_LEFT_X    230
#define POKE_CENTER_X  370
#define POKE_RIGHT_X   510

Collisions* newCollision(int x1, int y1, int x2, int y2) {
	Collisions* col = (Collisions*) malloc(sizeof(Collisions));
	col->x1 = x1;
	col->y1 = y1;
	col->x2 = x2;
	col->y2 = y2;
	return col;
}

int checkCollision(Collisions* col1, Collisions* col2) {
	if ((col2->x1 == OBS_LEFT_X || col2->x1 == POKE_LEFT_X)
			&& col1->x1 < col2->x2) {
		if (col1->y1 < col2->y2 && col1->y2 > col2->y2)
			return 1;
		return 0;
	} else if ((col2->x1 == OBS_CENTER_X || col2->x1 == POKE_CENTER_X)
			&& col1->x1 < col2->x1 && col1->x2 > col2->x1) {
		if (col1->y1 < col2->y2 && col1->y2 > col2->y2)
			return 1;
		return 0;
	} else if ((col2->x1 == OBS_CENTER_X || col2->x1 == POKE_CENTER_X)
			&& col1->x2 > col2->x2 && col1->x1 < col2->x2) {
		if (col1->y1 < col2->y2 && col1->y2 > col2->y2)
			return 1;
		return 0;
	} else if ((col2->x1 == OBS_CENTER_X || col2->x1 == POKE_CENTER_X)
			&& col1->x1 > col2->x1 && col1->x2 < col2->x2) {
		if (col1->y1 < col2->y2 && col1->y2 > col2->y2)
			return 1;
		return 0;
	} else if ((col2->x1 == OBS_RIGHT_X || col2->x1 == POKE_RIGHT_X)
			&& col1->x2 > col2->x1) {
		if (col1->y1 < col2->y2 && col1->y2 > col2->y2)
			return 1;
		return 0;
	}
	return 0;
}

void deleteCollision(Collisions* col) {
	free(col);
}
