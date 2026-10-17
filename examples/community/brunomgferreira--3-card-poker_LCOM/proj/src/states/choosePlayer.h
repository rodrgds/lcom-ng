#ifndef __CHOOSE_PLAYER_H_
#define __CHOOSE_PLAYER_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "states/main.h"
#include "states/menuOption.h"
#include "keyboard_mouse/keyboard.h"
#include "keyboard_mouse/mouse.h"
#include "video/video.h"
#include "sprites/sprite.h"

/**
 * @defgroup choose_player Choose Player
 * @{
 *
 * Functions and structures for handling player selection
 */

#define SCROLLER_X 920           /**< @brief X coordinate of the scroller */
#define SCROLLER_Y 340           /**< @brief Y coordinate of the scroller */
#define SCROLLER_WIDTH 10        /**< @brief Width of the scroller */
#define SCROLLER_HEIGHT (double) 340 /**< @brief Height of the scroller */
#define SCROLLER_THUMB_X SCROLLER_X - 10 /**< @brief X coordinate of the scroller thumb */
#define SCROLLER_THUMB_SIZE 30   /**< @brief Size of the scroller thumb */

#define REWARD 100               /**< @brief Reward for selecting a player */

/**
 * @brief Handles keyboard input during player selection
 */
void (handle_kbd_choose_player)();

/**
 * @brief Handles mouse input during player selection
 */
void (handle_mouse_choose_player)();

/**
 * @brief Handles timer interrupts during player selection
 */
void (handle_timer_choose_player)();

/**
 * @brief Checks if the mouse is on the scroller
 * @return true if the mouse is on the scroller, false otherwise
 */
bool (is_on_scroller)();

/**
 * @brief Checks if the mouse is on the scroller thumb
 * @return true if the mouse is on the scroller thumb, false otherwise
 */
bool (is_on_scroller_thumb)();

/**
 * @brief Updates the position of the scroller based on mouse input
 */
void (update_scroller_position)();

/**
 * @brief Handles mouse click events during player selection
 */
void (handle_choose_player_mouse_click)();

/**
 * @brief Handles mouse movement events during player selection
 */
void (handle_choose_player_mouse_movement)();

/**
 * @brief Selects a player based on mouse input
 * @param player Pointer to the selected player
 */
void (select_player)(struct Player *player);

/**
 * @brief Draws the players to select from
 */
void (draw_players)();

/**
 * @brief Draws the scroller for player selection
 */
void (draw_scroller)();

/** @} */

#endif /* __CHOOSE_PLAYER_H_ */
