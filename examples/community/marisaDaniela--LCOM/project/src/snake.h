#pragma once
#include "point.h"
#include "utils.h"
#include "fruit.h"
#include "i8042.h"

#include <minix/drivers.h>

#define INIT_SIZE 3
/** @defgroup snake snake
 * @{
 * Functions for manipulating the Snake
 */
typedef enum
{
	LEFT, RIGHT, UP, DOWN
} Direction;

/**
 * @brief Snake info struct
 */
typedef struct
{
	int score; 
	Direction direction;	// snake's direction
	unsigned int size; 		// snake's size
	Point** snakePosition;	// snake[0] is the tail snake[size - 1] is the head
} Snake;

/**
 * @brief Function to create a new snake to put on the screen
 */
Snake* initSnake();

/**
 * @brief Function to kill the snake
 * @param fp pointer to file
 * @param snake pointer to snake
 * @param fruit pointer to fruit
void die(FILE * fp, Snake * snake, Fruit * fruit);
*/

/**
 * @brief Function that updates the snake's position
 * @param fp pointer to file
 * @param snake pointer to snake
 * @param fruit pointer to fruit
 * @param specialF pointer to special fruit
void moveSnake(FILE * fp, Snake * snake, Fruit * fruit, Fruit * specialF);
*/

/**
 * @brief Function that prints the snake's position
 * @param snake pointer to snake
 */
void printSnakePosition(Snake* snake);

/**
 * @brief Function that updates the snake's direction
 * @param snake pointer to snake
 * @param key breakcode from keyboard
 */
void updateDirection(Snake* snake, unsigned short key);