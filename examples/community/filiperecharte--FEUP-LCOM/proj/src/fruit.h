#pragma once
#include "i8254.h"
#include "videocard.h"
#include "snake.h"

#include <time.h>
#include <stdlib.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/// Struct with fruit information
struct Fruit {
	unsigned short x_coordinate;
	unsigned short y_coordinate;
};
/**
* @brief Spawns fruit
*
* Spawns the fruits generated at randomly chosen positions
* A lot of verifications are done - for example - fruits cannot spawn inside the snake, inside the walls and directly in front of the snake's head
* 
* @param background_game - buffer of type void pointer
* @param xpm_game_background_adr - pointer to starting addresss of green background xpm
* @param apple_adr - pointer to starting addresss of apple xpm
* @param img_game_background - image of green background of type xpm_image_t
* @param img_apple - image of apple of type xpm_image_t
* @return Returns nothing
*/
void spawnFruit(void* background_game, uint8_t* xpm_game_background_adr, uint8_t* apple_adr, 
	xpm_image_t img_game_background, xpm_image_t img_apple);
/**
* @brief Checks collision
*
* Checks snake collision with fruits
*
* @return Returns true if snake is colliding, false otherwise
*/
bool checkColision();
