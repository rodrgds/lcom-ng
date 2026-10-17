/**
 * @file game
 * 
 * @brief This file contains functions that will draw the mouse on the screen when called, draw the game screen (the barriers, the obstacles, the grass, etc.),
 * if a user clicks on ESC it goes back to the main menu and it also as the logic for the golf ball's swing.
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023  
*/

#ifndef PROJ_GAME_H
#define PROJ_GAME_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <string.h>

#include "../devices/keyboard/i8042.h"
#include "../devices/video/video.h"
#include "../sprites/sprites.h"
#include "../devices/mouse/mouse.h"
#include "../MACROS.h"

/**
 * @brief this function draws the mouse on the screen at a (x, y) position based on the xpm
*/

void (drawMouse)();

/**
 * @brief this function draws the game menu
 * 
 * @return 0
*/

int drawGame();

/**
 * @brief Processes keyboard input from the user and changes view based on it.
 * 
 * @param scancode represents the key pressed by the user
*/

void process_keyboard_game(uint8_t scancode);


/**
 * @brief Function that will process a ball swing.
*/

void process_tacada_MOUSE ();

/**
 * @brief Function that will check if the ball is in the hole
*/

void checkIfBallInHole();

/**
 * @brief Will check if ball as a collision with a border in the x side.
 * 
 * @param xbola x coordinate of ball
 * 
 * @param ybola y coordinate of ball
 * 
 * @return true if collision false otherwise
*/

bool checkColisionx(int xbola, int ybola);

/**
 * @brief Will check if ball as a collision with a border in the y side.
 * 
 * @param xbola x coordinate of ball
 * 
 * @param ybola y coordinate of ball
 * 
 * @return true if collision false otherwise
*/

bool checkColisiony(int xbola, int ybola);

#endif //PROJ_GAME_H

