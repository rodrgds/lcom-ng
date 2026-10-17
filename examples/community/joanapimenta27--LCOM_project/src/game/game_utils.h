/**
 * @file game_utils.h
 * @brief Header file containing utility functions and constants for the game.
 */

#ifndef _LCOM_GAME_UTILS_H
#define _LCOM_GAME_UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "../xpm/xpm_letters.h"
#include "../xpm/xpm_nums.h"
#include "../xpm/xpm_player.h"
#include "../xpm/xpm_menu.h"
#include "../xpm/xpm_game.h"
#include "../xpm/xpm_levels.h"
#include "../xpm/xpm_targets.h"

#define GAME_FREQUENCY          30 /**< The frequency of the game loop in Hz. */

#define H_PSEUDO_RES            80 /**< The horizontal resolution of the game screen. */
#define V_PSEUDO_RES            60 /**< The vertical resolution of the game screen. */
#define PSEUDO_PIXEL_SIZE       10 /**< The size of each pseudo pixel on the game screen. */

#define NUMBER_OF_MENU_OPTIONS  2 /**< The number of options in the game menu. */

#define RANDINT(n)              (rand() % (n+1)); /**< Macro to generate a random integer between 0 and n. */
#define GAME_RAND_MAX           99 /**< The maximum value for the random number generator. */
#define MAX_TARGETS             15 /**< The maximum number of targets in the game. */

#define TARGET_CHANCE_MAX       10 /**< The maximum chance value for generating targets. */
#define TARGET_HIGH_CHANCE      2 /**< The chance value for generating high-value targets. */
#define TARGET_MID_CHANCE       4 /**< The chance value for generating mid-value targets. */

#define LEVEL_1_MIN_POINTS      50 /**< The minimum points required to reach level 1. */
#define LEVEL_2_MIN_POINTS      100 /**< The minimum points required to reach level 2. */
#define LEVEL_3_MIN_POINTS      200 /**< The minimum points required to reach level 3. */

#define KEYBOARD_Q              0x10 /**< The keyboard scan code for the 'Q' key. */
#define KEYBOARD_P              0x19 /**< The keyboard scan code for the 'P' key. */
#define KEYBOARD_ARROW_UP       0x48 /**< The keyboard scan code for the up arrow key. */
#define KEYBOARD_ARROW_DOWN     0x50 /**< The keyboard scan code for the down arrow key. */
#define KEYBOARD_ENTER_BREAK    0x9C /**< The keyboard scan code for the enter/break key. */
#define KEYBOARD_S              0x1F /**< The keyboard scan code for the 'S' key. */
#define KEYBOARD_W              0x11 /**< The keyboard scan code for the 'W' key. */
#define KEYBOARD_A              0x1E /**< The keyboard scan code for the 'A' key. */
#define KEYBOARD_D              0x20 /**< The keyboard scan code for the 'D' key. */

#define TILE_WALL               0 /**< The tile type for walls. */
#define TILE_HURT               1 /**< The tile type for hurt tiles. */
#define TILE_TELEPORT           2 /**< The tile type for teleport tiles. */
#define TILE_COIN               3 /**< The tile type for coin tiles. */

#define LEVEL_DURATION          120 /**< The duration of each level in seconds. */

/**
 * @brief Represents a position in the game grid.
 */
typedef struct {
  uint8_t x; /**< The x-coordinate of the position. */
  uint8_t y; /**< The y-coordinate of the position. */
} position_t;

/**
 * @brief Represents the system state of the game.
 */
typedef enum  {
  GAME_RUNNING, /**< The game is currently running. */
  GAME_EXIT /**< The game has exited. */
} sys_state_e;

/**
 * @brief Represents the different states of the game.
 */
typedef enum  {
  MENU, /**< The game is in the menu state. */
  PRE_LEVEL, /**< The game is in the pre-level state. */
  LEVEL, /**< The game is in the level state. */
  END_GAME, /**< The game is in the end-game state. */
  PAUSED, /**< The game is in the paused state. */
  WIN, /**< The game is in the win state. */
  DEAD, /**< The game is in the dead state. */
  SCOREBOARD /**< The game is in the scoreboard state. */
} game_state_e;

/**
 * @brief Represents the different states of the game levels.
 */
typedef enum  {
  NON, /**< No level is selected. */
  LEVEL_1, /**< Level 1 is selected. */
  LEVEL_2, /**< Level 2 is selected. */
  LEVEL_3 /**< Level 3 is selected. */
} level_state_e;

/**
 * @brief Represents the different options in the game menu.
 */
typedef enum {
  START, /**< The start option in the menu. */
  EXIT /**< The exit option in the menu. */
} menu_option_e;

#endif
