#include <stdlib.h>
#include "point.h"

Point* getPoint(int x, int y)
{
	Point* point = (Point *) malloc(sizeof(Point));

	point->x = x;
	point->y = y;

	return point;
}

Point* pointToCoord(Point* point)
{
	Point* coord = (Point *) malloc(sizeof(Point));

	coord->x = point->x * TILE_SIZE + BOARD_X + TILE_SIZE;
	coord->y = point->y * TILE_SIZE + BOARD_Y + TILE_SIZE;

	return coord;
}

int comparePoints(Point* a, Point* b)
{
	if((a->x == b->x) && (a->y == b->y))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
