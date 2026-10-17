#ifndef __MAIN_H_
#define __MAIN_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "rtc/rtc.h"

/**
 * @defgroup main Main
 * @{
 *
 * Functions and structures for controlling the main flow of the game
 */

/**
 * @brief Structure representing a player
 */
struct Player {
    char name[256]; /**< Name of the player */
    int coins; /**< Number of coins the player has */
    time_info nextRewardDataTime; /**< Time for the next reward */
};

#include "states/game/game.h"
#include "states/menu.h"
#include "states/choosePlayer.h"
#include "states/createPlayer.h"
#include "timer/timer.h"
#include "keyboard_mouse/i8042.h"
#include "keyboard_mouse/kbc.h"
#include "keyboard_mouse/keyboard.h"
#include "keyboard_mouse/mouse.h"
#include "sprites/sprite.h"

/**
 * @brief Enumeration representing the main states of the game
 */
typedef enum {
  MENU, /**< Menu state */
  GAME, /**< Game state */
  EXIT, /**< Exit state */
  CREATE_PLAYER, /**< Create player state */
  CHOOSE_PLAYER, /**< Choose player state */
} main_state_enum;

#define MAX_PLAYERS 100

/**
 * @brief Handles timer interrupt controller
 */
void (timer_int_controller)();

/**
 * @brief Handles keyboard input controller
 */
void (keyboard_input_controller)();

/**
 * @brief Handles mouse input controller
 */
void (mouse_input_controller)();

/**
 * @brief Checks if the mouse is on the close button
 * @return true if the mouse is on the close button, false otherwise
 */
bool is_on_close_button();

/**
 * @brief Draws the mouse cursor
 */
void (draw_mouse)();

/**
 * @brief Reads keyboard input as a string
 * @param scancode Scancode of the input
 * @param input_string String to store the input
 * @param string_length Pointer to the length of the string
 * @param max_string_length Maximum length of the string
 */
void (read_kbd_input_str)(uint8_t scancode, char input_string[], uint32_t *string_length, uint32_t max_string_length);

/**
 * @brief Converts scancode to ASCII character
 * @param scancode Scancode to convert
 * @return ASCII character corresponding to the scancode
 */
char (scancode_to_ascii)(uint8_t scancode);

/**
 * @brief Checks if a scancode represents a printable character
 * @param scancode Scancode to check
 * @return true if the scancode represents a printable character, false otherwise
 */
bool (is_printable)(uint8_t scancode);

/**
 * @brief Decodes a word and draws it on the screen
 * @param word Word to decode and draw
 * @param x X coordinate to draw the word
 * @param y Y coordinate to draw the word
 */
void (wordDecoder)(char* word, uint32_t x, uint32_t y);

/**
 * @brief Draws a text container on the screen
 * @param x X coordinate of the container
 * @param y Y coordinate of the container
 * @param width Width of the container
 * @param height Height of the container
 * @param color Color of the container
 */
void (draw_text_container)(int x, int y, int width, int height, uint32_t color);

/**
 * @brief Extracts the directory path from the program's path
 * @param dirpath Buffer to store the directory path
 */
void (extract_directory_path)(char *dirpath);

/**
 * @brief Writes player data to a file
 */
void (write_player_data_to_file)();

/**
 * @brief Reads player data from a file
 */
void (read_player_data_from_file)();

/** @} */

#endif /* __MAIN_H_ */
