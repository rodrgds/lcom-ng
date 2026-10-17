/**
 * @file menu.h
 * @brief Module for handling game menus and time display.
 */

#ifndef _MENU_H_
#define _MENU_H_

#include <lcom/lcf.h>

#include "cursor.h"
#include "sprites/sprite.h"
#include "gameElements/playerShip.h"
#include "gameElements/spaceship_explosion.h"
#include "gameElements/enemy.h"
#include "gameElements/fire.h"

#include "devices/RTC.h"

/** @defgroup menu Menu
 * @{
 *
 * @brief  Menu related functions
 * 
 */



/**
 * 
 * @brief Enum type for the existing Menus
 * 
 */
typedef enum {
    MAIN, /**< Main menu */
    LEADERBOARD, /**< Leaderboard menu */
    INSTRUCTIONS /**< Instructions menu */
} menu;

/**
 * @brief Enum type for the selected state of the menu
 */

typedef enum {
    INITIAL, /**< Initial state */
    HOVER_PLAY, /**< Play button hovered */
    HOVER_LEADER_BOARD, /**< Leaderboard button hovered */
    HOVER_INSTRUCTIONS,  /**< Instructions button hovered */
    HOVER_QUIT, /**< Quit button hovered */
    HOVER_BACK, /**< Back button hovered */
} menuSelectedState;

/**
 * @brief Draws the specified menu.
 * 
 * @param menu The menu to be drawn.
 */
void draw_menu(menu menu);

/**
 * @brief Draws the current time on the screen.
 */
void draw_time();

/** @} end of menu */

#endif /* _MENU_H_ */


