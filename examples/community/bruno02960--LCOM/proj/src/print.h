/**
 * @file print.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains menu and other print functions
 */

#ifndef __MENU_H
#define __MENU_H

/** @defgroup print print
 * @{
 *
 * Functions for non-object graphics display
 */

/*
 * @ brief Prints score and time information
 */
void print_info();

/*
 * @ brief Prints final score
 */
void print_score();

/*
 * @brief Prints message at the end of the game
 *
 * @param pixmap Pixmap with the message to be printed
 */
void print_message(char* pixmap[]);

/*
 * @brief Prints a number
 *
 * @param number Number to be printed
 * @param x Starting x coordinate
 * @param y Starting y coordinate
 */
void print_number(unsigned number, int x, int y);

/*
 * @brief Prints the Main Menu
 */
void print_menu();

/*
 * @brief Prints the game instructions
 */
void print_instructions();
/*
 * @brief Prints the Menu to choose Keyboard or Mouse
 */
void print_menu_kbd_or_mouse();

/** @} end of print */

#endif
