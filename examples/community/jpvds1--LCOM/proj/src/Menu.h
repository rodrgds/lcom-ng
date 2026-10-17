/**

@file Menu.h
@brief Header file containing declarations of functions used for controlling and displaying the different menus.

*/

#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include "proj.h"
#include "Levels.h"

/**
 * @brief deals with the timer, keyboard and mouse interrupts while on the menu
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int menu_interrupt_loop();
/**
 * @brief deals with the timer, keyboard and mouse interrupts while on the dead user menu
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int dead_menu_interrupt_loop();
/**
 * @brief deals with the timer, keyboard and mouse interrupts while on the instructions screen
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int instructions_loop();
/**
 * @brief draws menu elements
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int draw_menu();
/**
 * @brief draws the menu that appears when the user loses/finishes the game
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int dead_draw_menu();

#endif //MENU_H
