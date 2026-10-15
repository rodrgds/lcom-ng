#include "Position.h"
#include <stdlib.h>


Position *newPosition(int x, int y) {
    Position* position = (Position *) malloc(sizeof(Position));
    position->x = x;
    position->y = y;
    return position;
}


void deletePosition(Position* position) {
    free(position);
}
