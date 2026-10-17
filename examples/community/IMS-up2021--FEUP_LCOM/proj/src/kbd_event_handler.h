#include <stdio.h>
#include "kbd.h"
#include "game_state.h"

/**
 * @brief Handles generic keyboard events.
 *
 * This function processes keyboard input events for the generic state of the game or 
 * application, updating the state based on the provided scancode.
 *
 * @param scan_code An array of two bytes representing the keyboard scancode.
 */
void (handle_kbd_event)(uint8_t scan_code[2]);

/**
 * @brief Handles keyboard events during the game.
 *
 * This function processes keyboard input events specifically when the game is in 
 * the playing state, updating the game's state based on the provided scancode.
 *
 * @param scan_code An array of two bytes representing the keyboard scancode.
 */
void (handle_kbd_game_event)(uint8_t scan_code[2]);

/**
 * @brief Handles keyboard events during the instructions screen.
 *
 * This function processes keyboard input events specifically when the game is displaying 
 * instructions, updating the state based on the provided scancode.
 *
 * @param scan_code An array of two bytes representing the keyboard scancode.
 */
void (handle_kbd_instructions_event)(uint8_t scan_code[2]);
