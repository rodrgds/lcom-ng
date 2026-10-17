#pragma once

#include "snake.h"
#include <minix/syslib.h>

/**
 * @brief fruit info struct
 */
typedef struct
{
	Point* fruitCoords;		
	Point* fruitPosition;	
	int value; 
	unsigned int duration; 
	int special; 
	unsigned int timer;	
} Fruit;


/**
 * @brief Function to create a new fruit to place randomly on the screen
 */
Fruit* initFruit();

/**
 * @brief Function to snake eat fruit when reaches it
 * @return fruit pontuation
 */
int eatFruit();

/**
 * @brief Function that updates the position of the fruit
 */
void updatepositionF(Fruit* fruit);

/**
 * @brief Initializes a new fruit
 * @return pointer to the new fruit created
 */
Fruit* specialFruit(); 

