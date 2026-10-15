/**
 * @file record
 * @brief Functions for the game record
 */

#ifndef _LCOM_RECORD_H_
#define _LCOM_RECORD_H_

/** @defgroup Record Record
 * @{
 *
 * Functions for the game record
 */

#include <stdio.h>

#include "../main_game/main_game.h"
#include "../../devices/rtc/rtc.h"

/**
 * @brief Initiates the winner mode setting the name and allowing the keyboard to work
 *
 */
void (winner_init)();

/**
 * @brief Display on the screen the elements to create the winner page
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (display_winner_page)();

/**
 * @brief Display on the screen the elements to create the record page
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (display_record_page)();

/**
 * @brief Display on the screen the 5 last winners of the game
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (display_winners)();

/**
 * @brief Display on the screen a winner 
 *
 * @param winner_record The name of the winner
 * @param game_duration_record The duration in seconds the winner took to win the game
 * @param date_time_record The date and time when the winner won the game
 * @param x The x position to display all the informations starting from the left to right
 * @param y The y position to display all the informations starting from the left to right
 * @return Return 0 upon success and 1 otherwise
 */
int (display_winner)(char winner_record[10], int game_duration_record, char date_time_record[20], int x, int y);

/**
 * @brief Save the player name, the time duration and the date/time when a player won the game in a file
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (save_winner)();

/**
 * @brief Controls what the keyboard can do in the winner page
 *
 * @param bytes The scancode read from the keyboard
 * @return Return 0 upon success and 1 otherwise
 */
int (winner_control_kbc)(uint8_t *bytes);

/**
 * @brief Controls what the keyboard can do in the record page
 *
 * @param bytes The scancode read from the keyboard
 */
void (record_control_kbc)(uint8_t *bytes);

/**
 * @brief Controls what the mouse can do in the winner page
 *
 * @param pp The mouse parsed packet to check its click
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 * @return Return 0 upon success and 1 otherwise
 */
int (winner_control_mouse)(struct packet *pp, int x, int y);

/**
 * @brief Controls what the mouse can do in the record page
 *
 * @param pp The mouse parsed packet to check its click
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 */
void (record_control_mouse)(struct packet *pp, int x, int y);

/**
 * @brief Display the mouse used for navigate on the winner page
 *
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 * @return Return 0 upon success and 1 otherwise
 */
int (winner_display_mouse)(int x, int y);

/**
 * @brief Display the mouse used for navigate on the record page
 *
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 * @return Return 0 upon success and 1 otherwise
 */
int (record_display_mouse)(int x, int y);

/**@}*/

#endif
