/**
 * This module is responsible for handling all interrupt devices, drawing images, update states, throw events
 * Jorge Vale: 20%
 * Telmo Barros: 80%
 * Relative Weight: 17%
 */
#ifndef __HANDLER_H
#define __HANDLER_H

#include "game.h"
#include "menu.h"
#include "character.h"

/** @defgroup Handler Handler
 * @{
 * Functions for handling all interrupt devices, drawing images, update states, throw events
 */

#define		RTC_TIME_X			7
#define		RTC_TIME_Y			724

/**
 * @brief Subscribe all interrupts and generate needed structure for all execution, calls all the functions, gets highscores registers
 *
 * @return 0 upon success
 */
int mainhandler();

/**
 * @brief Handle the mouse interrupts, updating mouse position, delta and buttons flags
 *
 * @return 0 upon success
 */
int mouse_packet_handler();

/**
 * @brief Handle the events based on the mouse position and left button click
 *
 * @param menu Menu to check buttons positions
 *
 * @return Event of main_menu
 */
main_menu_event main_menu_event_handler(Menu* menu);

/**
 * @brief Handle the next mode to show based on menu state
 *
 * @param menu Menu to check state
 *
 * @return Event of main_menu
 */
void selecao_handler(Menu* menu);

/**
 * @brief Creates a menu and updates its events, states, draws the menu and the mouse, handle all devices interrupts
 *
 * @return 0 upon success
 */
int menu_handler();

/**
 * @brief Handle the keyboard interrupts, updating actual code (key code)
 *
 * @return 0 upon success
 */
int kbd_int_handler();

/**
 * @brief Handle the keyboard game events based on the actual key code
 *
 * @return Event of game
 */
kbd_game_event kbd_event_handler();

/**
 * @brief Handle the mouse game events based on the delta x and buttons flags
 *
 * @return Event of game
 */
mouse_game_event mouse_event_handler();

/**
 * @brief Handle the timer game events based on periodic interrupts to make piece fall
 *
 * @param counter Actual tick counter
 * @param delay Actual fall delay
 *
 * @return Event of game
 */
timer_game_event timer_event_handler(unsigned long counter, unsigned int delay);

/**
 * @brief Creates a game and updates its events, states, draws the game, counter and handle all devices interrupts
 *
 * @return 0 upon success
 */
int game_handler();

/**
 * @brief Handle the serial interrupts, checking its origin and reading the value in correspondent register
 *
 * @param temp Variable to change with the value read from serial port
 *
 * @return interrupt origin
 */
int serial_int_handler(unsigned long * temp);

/**
 * @brief Creates a game and updates its events, states, draws the game, counter and handle all devices interrupts
 *
 * @return 0 upon success
 */
int multi_game_handler();

/**
 * @brief Handle the keyboard character events based on the actual key code
 *
 * @return Event of character
 */
kbd_character_event kbd_char_event_handler();

/**
 * @brief Handle the timer character events based on periodic interrupts to animate sprite
 *
 * @param counter Actual tick counter
 *
 * @return Event of character
 */
timer_character_event timer_char_event_handler(unsigned long * counter);

/**
 * @brief Creates a game and a character and updates their events, states, draws the game and the character, counter and handle all devices interrupts
 *
 * @return 0 upon success
 */
int battle_game_handler();

/**
 * @brief Shows highscores by getting an updated highscores array
 *
 * @return 0 upon success
 */
int highscores_handler();

/**
 * @brief Draws the four images of instructions and handle keyboard in order to change image showed
 *
 * @return 0 upon success
 */
int instructions_handler();

/**@}*/

#endif /* __HANDLER_H */
