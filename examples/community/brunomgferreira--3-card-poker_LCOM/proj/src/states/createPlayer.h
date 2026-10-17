#ifndef __CREATE_PLAYER_H_
#define __CREATE_PLAYER_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "states/main.h"
#include "keyboard_mouse/keyboard.h"
#include "keyboard_mouse/mouse.h"
#include "video/video.h"
#include "sprites/sprite.h"

/**
 * @defgroup create_player Create Player
 * @{
 *
 * Functions and structures for creating new players
 */

#define CREATE_PLAYER_BTN_X 400          /**< @brief X coordinate of the create player button */
#define CREATE_PLAYER_BTN_Y 560          /**< @brief Y coordinate of the create player button */
#define CREATE_PLAYER_BTN_WIDTH 350      /**< @brief Width of the create player button */
#define CREATE_PLAYER_BTN_HEIGHT 55      /**< @brief Height of the create player button */
#define CREATE_PLAYER_INPUT_X 350        /**< @brief X coordinate of the create player input field */
#define CREATE_PLAYER_INPUT_Y 370        /**< @brief Y coordinate of the create player input field */


/**
 * @brief Handles keyboard input during player creation
 *
 * This function handles keyboard input during the creation of a new player.
 */
void handle_kbd_create_player();

/**
 * @brief Handles mouse input during player creation
 *
 * This function handles mouse input during the creation of a new player.
 */
void handle_mouse_create_player();

/**
 * @brief Handles timer interrupts during player creation
 *
 * This function handles timer interrupts during the creation of a new player.
 */
void handle_timer_create_player();

/**
 * @brief Creates a new player based on input
 *
 * This function creates a new player based on the input provided.
 */
void create_new_player();

/** @} */

#endif /* __CREATE_PLAYER_H_ */
