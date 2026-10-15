/**
 * @file game_utils
 * @brief Functions for support the game
 */

#ifndef _LCOM_GAME_UTILS_H_
#define _LCOM_GAME_UTILS_H_

/** @defgroup Game_Utils Game_Utils
 * @{
 *
 * Functions for support the game 
 */

#include "../../images/sprite/sprite.h"
#include "../card/card.h"

Sprite *game_title;
Sprite *record_title;
Sprite *congratulations;
Sprite *start_option;
Sprite *exit_option;
Sprite *record_option;
Sprite *save_option;
Sprite *x_option;

Sprite *background;

Sprite *mouse_cursor;
Sprite *select_mouse_cursor;

Sprite *symbol_1;
Sprite *symbol_2;
Sprite *symbol_3;
Sprite *symbol_4;
Sprite *symbol_5;
Sprite *symbol_6;
Sprite *symbol_7;
Sprite *symbol_8;

Sprite *symbol_cover;
Sprite *symbol_cover_selected;

Card *card_1;
Card *card_2;
Card *card_3;
Card *card_4;
Card *card_5;
Card *card_6;
Card *card_7;
Card *card_8;
Card *card_9;
Card *card_10;
Card *card_11;
Card *card_12;
Card *card_13;
Card *card_14;
Card *card_15;
Card *card_16;

/**
 * @brief Creates all the sprites needed in the game
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (create_all_sprites)();

/**
 * @brief Destroy all the sprites created in the game freeing their memory
 *
 */
void (destroy_all_sprites)();

/**
 * @brief Creates all cards that will be used in the game
 *
 * @return Return 0 upon success and 1 otherwise
 */
int (create_all_cards)();

/**
 * @brief Destroy all cards created in the game freeing their memory
 *
 */
void (destroy_all_cards)();

/**
 * @brief Initialize the game board with the cards placed randomly
 *
 * @param board The game board that will be filled with the cards
 * @param board_h_v_size The horizontal and vertical size of the board (square board)
 */
void (initialize_board)(Card *board[4][4], unsigned int board_h_v_size);

/**
 * @brief Display a number on the screen
 *
 * @param number The number that will be displayed
 * @param x The x position of the upper left corner of the number image
 * @param y The y position of the upper left corner of the number image
 */
int (display_number)(int number, int x, int y);

/**
 * @brief Display a string on the screen
 *
 * @param text The string that will be displayed
 * @param x The x position of the upper left corner of the char symbol in the string
 * @param y The y position of the upper left corner of the char symbol int the string
 */
int (display_string)(char* text, int x, int y);

/**@}*/

#endif
