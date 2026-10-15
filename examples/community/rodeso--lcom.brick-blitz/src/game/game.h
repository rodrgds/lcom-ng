/**
 * @file game.h
 * @brief Header file for the game module.
 */

#pragma once
 
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <lcom/lcf.h>
#include "objects.h"
#include "draw.h"

#include "dvcs/graphics/vbe.h"
#include "dvcs/keyboard/kbc.h"
#include "dvcs/timer/pit.h"
#include "dvcs/mouse/ps2.h"

#define A_KEY 0x1E
#define D_KEY 0x20
#define SPACE_KEY 0x39
#define ESC_KEY 0x01
#define ENTER_KEY 0x1C

/**
 * @brief Enumeration representing the different states of the game.
 */
typedef enum {
    MENU,   
    GAME,
    LOST,
    WON,
    EXIT
} State;

/**
 * @brief Runs the game.
 * @return 0 upon success, non-zero otherwise.
 */
int run();

/**
 * @brief Initializes the drawing module.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_init();

/**
 * @brief Draws a frame of the game.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_frame();

/**
 * @brief Moves the ball in the game.
 * @return 0 upon success, non-zero otherwise.
 */
int move_ball();

/**
 * @brief Prepares the video mode for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int prepare_video();

/**
 * @brief Disables the video mode for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int disable_video();

/**
 * @brief Prepares the keyboard for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int prepare_keyboard();

/**
 * @brief Disables the keyboard for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int disable_keyboard();

/**
 * @brief Handles keyboard input during the game.
 */
void handle_keyboard();

/**
 * @brief Prepares the timer for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int prepare_timer();

/**
 * @brief Disables the timer for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int disable_timer();

/**
 * @brief Prepares the mouse for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int prepare_mouse();

/**
 * @brief Disables the mouse for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int disable_mouse();

/**
 * @brief Prepares the screens for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int prepare_screens();

/**
 * @brief Prepares the objects for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int prepare_objects();

/**
 * @brief Launches a projectile in the game.
 * @return 0 upon success, non-zero otherwise.
 */
int projectileLaunch();

/**
 * @brief Moves the projectile in the game.
 * @return 0 upon success, non-zero otherwise.
 */
int move_projectile();

/**
 * @brief Moves the bricks in the game.
 * @return 0 upon success, non-zero otherwise.
 */
int move_bricks();

/**
 * @brief Moves the marker in the game.
 * @return 0 upon success, non-zero otherwise.
 */
int move_marker();
