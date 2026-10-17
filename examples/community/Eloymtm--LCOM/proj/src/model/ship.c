#include "ship.h"
#include <string.h>


void (initializeShip)(Ship *ship, int size, Coordinate initial_coordinate, bool vertical) {

    ship->size = size;
    ship->initial_coordinates = initial_coordinate;
    ship->vertical = vertical;

}
