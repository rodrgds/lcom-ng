/**
 * @file main_game
 * @brief Functions specific for the game scenario
 */

#ifndef _LCOM_MAIN_GAME_H_
#define _LCOM_MAIN_GAME_H_

/** @defgroup Main_Game Main_Game
 * @{
 *
 * Functions specific for the game scenario
 */

#include "../game_definition.h"
#include "../../devices/keyboard/kbc.h"
#include "../game_utils/game_utils.h"

/**
 * @brief Initiates the game
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (init_game)();

/**
 * @brief Display on the screen the elements to create the game page
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (main_game_display_page)();

/**
 * @brief Controls the game logic
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (main_game_control)();

/**
 * @brief Verify the cards selected by the player to check if it's a match
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (verify_selected_cards)();

/**
 * @brief Increase the game time each second
 *
 */
void (main_game_timer)();

/**
 * @brief Display the mouse used for play the game
 *
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 * @return Return 0 upon success and 1 otherwise
 */
int (main_game_display_mouse)(int x, int y);

/**
 * @brief Controls what the mouse can do when playing the game
 *
 * @param pp The mouse parsed packet to check its click
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 */
void (main_game_control_mouse)(struct packet *pp, int x, int y);

/**
 * @brief Select the card where the mouse is over
 *
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 * @return Return a pointer to the card where the mouse is over. If the mouse is not over any card, return null
 */
Card *(select_card)(int x, int y);

/**
 * @brief Unturn the cards selected by the player
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (unturn_selected_cards)();

/**
 * @brief Display the cards in the screen
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (display_cards)();

/**
 * @brief Controls what the keyboard can do when playing the game
 *
 * @param bytes The scancode read from the keyboard
 */
void (main_game_control_kbc)(uint8_t *bytes);

/**@}*/

#endif
