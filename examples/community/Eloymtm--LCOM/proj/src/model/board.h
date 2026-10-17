#ifndef BOARD_H
#define BOARD_H

#include "ship.h"
#include "coordinate.h"
#include "stdio.h"
#include "../macros.h"

#include <stdlib.h>

#define WATER '~' // Água
#define SHIP 'S'  // Navio
#define HIT  'X'  // Acerto
#define MISS 'O'  // Tiro na água
#define BOARD_SIZE 10

typedef struct {
    char** matrix; 
    Ship* ships;                       
    int num_ships;
    int selected;     
    Ship hit;                  
} Board;

/**
 * @brief Initializes a new game board.
 * 
 * This function dynamically allocates memory for a new Board structure, initializes
 * the matrix representing the game board with WATER tiles, initializes the hit structure,
 * and initializes the ships array with default ship configurations.
 * 
 * @return A pointer to the newly initialized Board structure.
 */
Board* (initializeBoard)();

/**
 * @brief Checks if a specific coordinate on the board is empty.
 * 
 * This function checks if the tile at the specified coordinate on the board is empty,
 * i.e., if it contains water.
 * 
 * @param board The Board structure representing the game board.
 * @param coordinate The Coordinate structure representing the coordinate to check.
 * @return Returns true if the specified coordinate is empty (contains water), otherwise false.
 */
bool (isEmpty)(Board board, Coordinate coordinate);

/**
 * @brief Checks if a ship can be rotated on the board.
 * 
 * This function checks if a ship can be rotated to change its orientation on the board without
 * colliding with other ships or going out of bounds.
 * 
 * @param board A pointer to the Board structure representing the game board.
 * @param ship A pointer to the Ship structure representing the ship to be rotated.
 * @return Returns true if the ship can be rotated, otherwise false.
 */
bool (canRotate)(Board* board, Ship *ship);
Coordinate (getInitialCoor)(int shipN);

bool(canMove)(Board* board, Ship *ship, int direction);
bool(targetCanMove)(Board* board, int direction);

int (lastShip)(Board *board);
int (firstShip)(Board *board);
bool (rotateShip)(Board* board, Ship *ship);

Coordinate (firstEmptyPosition)(Board *board, int ship_size);
Coordinate (nextEmptyPosition)(Board *board, Ship ship);
Coordinate (firstEmptyHitPosition)(Board *board);
Coordinate (moveSelected)(Board *Board,Coordinate pos, int scancode);

bool (isNoInArray)(int x,int y,Coordinate array[], int size);

bool (areAllShipsGuessed)(Board *board);

//int (registerShot)(Board *board, Coordinate target);

#endif // BOARD_H
