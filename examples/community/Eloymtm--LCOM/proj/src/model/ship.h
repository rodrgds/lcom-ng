#ifndef SHIP_H
#define SHIP_H

#include <stdbool.h>

#include "coordinate.h"



typedef struct {
    int size; // tamanho do barco (nº quadrados)
    Coordinate initial_coordinates; 
    bool vertical; // vertical ou horizontal
} Ship;

void (initializeShip)(Ship *ship, int size,Coordinate inital_coordinate, bool vertical);

#endif // SHIP_H
