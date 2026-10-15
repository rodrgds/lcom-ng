/**
 * @file constants.h
 * @author Bernardo Santos (bernardo.ots@fe.up.pt)
 * @brief Game constants and enumerations
 * @version 0.1
 * @date 2021-01-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CONSTANTS_H
#define __CONSTANTS_H

/** @defgroup constants constants
 * @{
 *
 * Game constants and enumerations
 */

/**
 * @brief Game state.
 * 
 */
typedef enum { MAIN_MENU,
               INSTRUCTIONS_MENU,
               ONGOING,
               FINISHED,
               LEAVE } state_t;

/**
 * @brief Game event. 
 * 
 */
typedef enum { INIT,
               START_GAME,
               INSTRUCTIONS,
               EXIT,
               GO_BACK_TO_MAIN_MENU,
               END_GAME } event_t;

/**
 * @brief Winner of the game.
 * 
 */
typedef enum { RED,
               BLUE,
               DRAW } winner;

/**
 * @brief Direction that a sprite is following.
 * 
 */
typedef enum { LEFT,
               RIGHT,
               UP,
               DOWN,
               NO } direction;

#define NUM_ELEMENTS 3

#define RED_PLAYER_IDX 0
#define BLUE_PLAYER_IDX 1
#define BALL_IDX 2

#define TRANSPARENCY_COLOR 0x0027FB09

#define FIELD_LEFT_LIMIT_PX 143
#define FIELD_RIGHT_LIMIT_PX 1012
#define FIELD_TOP_LIMIT_PX 209
#define FIELD_BOTTOM_LIMIT_PX 667

#define GOAL_TOP_POST_LIMIT_PX 324
#define GOAL_BOTTOM_POST_LIMIT_PX 551

#define START_GAME_BUTTON_LEFT_LIMIT_PX 320
#define START_GAME_BUTTON_RIGHT_LIMIT_PX 820
#define START_GAME_BUTTON_TOP_LIMIT_PX 375
#define START_GAME_BUTTON_BOTTOM_LIMIT_PX 488

#define INSTRUCTIONS_BUTTON_LEFT_LIMIT_PX 320
#define INSTRUCTIONS_BUTTON_RIGHT_LIMIT_PX 820
#define INSTRUCTIONS_BUTTON_TOP_LIMIT_PX 539
#define INSTRUCTIONS_BUTTON_BOTTOM_LIMIT_PX 651

#define EXIT_BUTTON_LEFT_LIMIT_PX 1067
#define EXIT_BUTTON_RIGHT_LIMIT_PX 1135
#define EXIT_BUTTON_TOP_LIMIT_PX 21
#define EXIT_BUTTON_BOTTOM_LIMIT_PX 90

#define GO_BACK_BUTTON_LEFT_LIMIT_PX 1060
#define GO_BACK_BUTTON_RIGHT_LIMIT_PX 1132
#define GO_BACK_BUTTON_TOP_LIMIT_PX 18
#define GO_BACK_BUTTON_BOTTOM_LIMIT_PX 89

/** @} end of constants */

#endif
