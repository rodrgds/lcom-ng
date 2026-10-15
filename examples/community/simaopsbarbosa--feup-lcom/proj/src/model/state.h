/**
 * @file state.h
 * @brief Interface for global game state management.
 */

#ifndef _LCOM_STATE_H_
#define _LCOM_STATE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "controllers/KBC/keyboard.h"
#include "controllers/KBC/mouse.h"
#include "controllers/video/video.h"
#include "view/view.h"

/**
 * @enum GameState
 * @brief Represents the current state of the game.
 */
typedef enum {
    RUNNING, /**< The game is running normally */
    EXIT,    /**< The game should exit */
} GameState;

/**
 * @brief Updates the game state related to the timer interrupt.
 *
 * This function should be called on timer interrupts to update
 * internal counters or perform timing-related logic.
 */
void update_timer_state();

/**
 * @brief Updates the game state related to keyboard input.
 *
 * Processes keyboard interrupts and updates
 * the game state accordingly.
 */
void update_keyboard_state();

/**
 * @brief Updates the game state related to mouse input.
 *
 * Processes mouse interrupts and synchronizes mouse data.
 */
void update_mouse_state();

/**
 * @brief Updates the mouse state specifically for menu interactions.
 *
 * Handles mouse movement and cursor positioning within menu screens,
 * applying sensitivity scaling and screen boundary constraints.
 */
void update_mouse_state_menus();

#endif /* _LCOM_STATE_H_ */
