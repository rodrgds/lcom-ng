#pragma once
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "i8254.h"

#include "keyboard.h"
#include "videocard.h"
#include "mouse.h"
#include "snake.h"
#include "fruit.h"
#include "videocard.h"

#include "chars.xpm"
/// Enum representing state of the program
enum game_state { MAIN_MENU, PLAY_HIGHLIGHT, LEADERBOARD_HIGHLIGHT, LEADERBOARD, EXIT_HIGHLIGHT, PLAY_BEFORE_KEY, PLAY, GAME_OVER, PAUSE_MENU, EXIT, DODGE, USERNAME };
/**
* @brief Mouse state machine handler
*
* Handles program's state depending on the user's actions
*
* @param mouse_img - image of mouse pointer of type xpm_image_t
* @param mouse_adr - pointer to starting addresss of mouse pointer xpm
* @param state - pointer to state of enum game_state type
* @param pkt - pointer to packet of struck packet type
* @param background_game - buffer of type void pointer 
* @param apple_adr - pointer to starting addresss of apple xpm
* @param img_apple - image of apple of type xpm_image_t
* @param xpm_game_background_adr - pointer to starting addresss of green background xpm
* @param img_game_background - image of green background of type xpm_image_t
* @return Returns nothing
*/
void mouse_event_handler(xpm_image_t mouse_img, uint8_t* mouse_adr,enum game_state* state, 
	struct packet* pkt, void* background_game, uint8_t *apple_adr, xpm_image_t img_apple, 
	uint8_t* xpm_game_background_adr, xpm_image_t img_game_background);
/**
* @brief Keyboard state machine handler
*
* Handles program's state depending on the user's actions
* @param state - pointer to state of enum game_state type
* @param key_pressed - pointer to the key the user pressed, represented as a 32 bit unsigned integer
* @param dir - pointer to the direction of snake as an enum direction type
* @return Returns nothing
*/
void keyboard_event_handler(enum game_state* state, uint32_t* key_pressed, enum direction* dir);
/**
* @brief Converts a string to characters printed to the screen
*
* Prints the string to the screen, starting at the given coordinates and inserting a newline after newline characters
*
* @param text - string to convert
* @param newline - insert a newline after this number of characters in the given string
* @param x - x coordinate to start drawing
* @param y - y coordinate to start drawing
* @return Returns nothing
*/
void text_to_screen(char text[], unsigned int newline, unsigned int x, unsigned int y);
/**
* @brief Loads alphabtet
*
* Loads the alphabet by loading all the consisting character's xpms
*
* @return Returns nothing
*/
void loadAlphabet();
