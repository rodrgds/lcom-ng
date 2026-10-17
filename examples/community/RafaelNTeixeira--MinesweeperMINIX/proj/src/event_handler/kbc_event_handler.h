#pragma once

#include <stdio.h>
#include "devices/keyboard/kbc.h"
#include "game/game_state.h"

/**
 * @brief This function handles keyboard events by dispatching them to the appropriate event handler based on the current state.
 * @param scan_code An array of two bytes representing the scan code of the key pressed on the keyboard.
*/
void handle_kbd_event(uint8_t scan_code[2]);

/**
 * @brief This function handles keyboard events specific to the main menu state.
 * @param scan_code An array of two bytes representing the scan code of the key pressed on the keyboard.
*/
void handle_kbd_mainmenu_event(uint8_t scan_code[2]);

/**
 * @brief This function handles keyboard events specific to the beginner state.
 * @param scan_code An array of two bytes representing the scan code of the key pressed on the keyboard.
*/
void handle_kbd_beginner_event(uint8_t scan_code[2]);

/**
 * @brief This function handles keyboard events specific to the medium state.
 * @param scan_code An array of two bytes representing the scan code of the key pressed on the keyboard.
*/
void handle_kbd_medium_event(uint8_t scan_code[2]);

/**
 * @brief This function handles keyboard events specific to the hard state.
 * @param scan_code An array of two bytes representing the scan code of the key pressed on the keyboard.
*/
void handle_kbd_hard_event(uint8_t scan_code[2]);


/**
 * @brief This function handles keyboard events specific to the instructions state.
 * @param scan_code An array of two bytes representing the scan code of the key pressed on the keyboard.
*/
void handle_kbd_instructions_event(uint8_t scan_code[2]);

/**
 * @brief This function handles the keyboard event when the game is won.
 * @param scan_code An array of two bytes representing the scan code of the key pressed on the keyboard.
*/
void handle_kbd_gamewin_event(uint8_t scan_code[2]);

/**
 * @brief This function handles the keyboard event when the game is over.
 * @param scan_code An array of two bytes representing the scan code of the key pressed on the keyboard.
*/
void handle_kbd_gameover_event(uint8_t scan_code[2]);
