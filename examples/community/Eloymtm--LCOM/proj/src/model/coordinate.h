#ifndef COORDINATE_H
#define COORDINATE_H

typedef struct {
    int row;
    int col;
} Coordinate;

void (initializeCoordinate)(Coordinate *coord, int row, int col);

#endif // COORDINATE_H
