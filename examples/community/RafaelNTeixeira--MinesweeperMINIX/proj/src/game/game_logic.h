#pragma once

#include <stdlib.h>

#include "game/game_state.h"

/**
 * @brief This function is used to calculate the matrix index based on a given x-coordinate.
 * @param x The x-coordinate for which to calculate the matrix index.
 * @return The matrix index corresponding to the given x-coordinate.
*/
int getMatrixIndexX(int x);

/**
 * @brief This function is used to calculate the matrix index based on a given y-coordinate.
 * @param x The y-coordinate for which to calculate the matrix index.
 * @return The matrix index corresponding to the given y-coordinate.
*/
int getMatrixIndexY(int y);

/**
 * @brief This function calculates the x-coordinate of a pressed cell in a 16x16 grid.
 * @param x The starting x-coordinate of the grid.
 * @return The x-coordinate of the pressed cell in the 16x16 grid.
*/
int getXCoordenatesPressedCell_16x16(int x);

/**
 * @brief This function calculates the y-coordinate of a pressed cell in a 16x16 grid.
 * @param y The starting x-coordinate of the grid.
 * @return The y-coordinate of the pressed cell in the 16x16 grid.
*/
int getYCoordenatesPressedCell_16x16(int y);


/**
 * @brief This function is used to randomly place mines on a 16x16 board.
 * @param board The 16x16 board represented by a 2D character array.
 * @param mines The number of mines to be placed on the board.
*/
void placeMines_16x16(char board[16][16], int mines);

/**
 * @brief This function is used to initialize a 16x16 board with default values.
 * @param board The 16x16 board represented by a 2D character array.
*/
void initializeBoard_16x16(char board[16][16]);

/**
 * @brief This function calculates and returns the count of adjacent mines around a specified cell in a 16x16 board.
 * @param board The 16x16 board represented by a 2D character array.
 * @param row The row index of the cell.
 * @param col The column index of the cell.
 * @return character representing the count of adjacent mines as a digit character (0 to 8).
*/
char countAdjacentMines_16x16(char board[16][16], int row, int col);

/**
 * @brief This function checks if the game is won in a 16x16 board.
 * @param board The 16x16 board represented by a 2D character array.
 * @return 1 if the game is won, and 0 otherwise.
*/
int winGame_16x16(char board[16][16]);

/**
 * @brief This function is responsible for checking and revealing empty plates in a 16x16 board.
 * @param board The 16x16 board represented by a 2D character array.
 * @param x The row index of the current cell.
 * @param y The column index of the current cell.
*/
void checkEmptyPlates_16x16(char board[16][16], int x, int y);


/**
 * @brief This function is used to randomly place mines on a 20x16 board.
 * @param board The 20x16 board represented by a 2D character array.
 * @param mines The number of mines to be placed on the board.
*/
void placeMines_20x16(char board[20][16], int mines);

/**
 * @brief This function is used to initialize a 20x16 board with default values.
 * @param board The 20x16 board represented by a 2D character array.
*/
void initializeBoard_20x16(char board[20][16]);

/**
 * @brief This function calculates and returns the count of adjacent mines around a specified cell in a 20x16 board.
 * @param board The 20x16 board represented by a 2D character array.
 * @param row The row index of the cell.
 * @param col The column index of the cell.
 * @return character representing the count of adjacent mines as a digit character (0 to 8).
*/
char countAdjacentMines_20x16(char board[20][16], int row, int col);

/**
 * @brief This function checks if the game is won in a 20x16 board.
 * @param board The 20x16 board represented by a 2D character array.
 * @return 1 if the game is won, and 0 otherwise.
*/
int winGame_20x16(char board[20][16]);

/**
 * @brief This function is responsible for checking and revealing empty plates in a 20x16 board.
 * @param board The 20x16 board represented by a 2D character array.
 * @param x The row index of the current cell.
 * @param y The column index of the current cell.
*/
void checkEmptyPlates_20x16(char board[20][16], int x, int y);


/**
 * @brief This function is used to randomly place mines on a 28x20 board.
 * @param board The 28x20 board represented by a 2D character array.
 * @param mines The number of mines to be placed on the board.
*/
void placeMines_28x20(char board[28][20], int mines);

/**
 * @brief This function is used to initialize a 28x20 board with default values.
 * @param board The 28x20 board represented by a 2D character array.
*/
void initializeBoard_28x20(char board[28][20]);

/**
 * @brief This function calculates and returns the count of adjacent mines around a specified cell in a 28x20 board.
 * @param board The 28x20 board represented by a 2D character array.
 * @param row The row index of the cell.
 * @param col The column index of the cell.
 * @return character representing the count of adjacent mines as a digit character (0 to 8).
*/
char countAdjacentMines_28x20(char board[28][20], int row, int col);

/**
 * @brief This function checks if the game is won in a 28x20 board.
 * @param board The 28x20 board represented by a 2D character array.
 * @return 1 if the game is won, and 0 otherwise.
*/
int winGame_28x20(char board[28][20]);

/**
 * @brief This function is responsible for checking and revealing empty plates in a 28x20 board.
 * @param board The 28x20 board represented by a 2D character array.
 * @param x The row index of the current cell.
 * @param y The column index of the current cell.
*/
void checkEmptyPlates_28x20(char board[28][20], int x, int y);

