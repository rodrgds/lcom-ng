#ifndef MANAGER_H
#define MANAGER_H

/**
 * @file manager.h
 * @brief Central management for game initialization, event handling, and state transitions.
 *
 * This file declares global variables related to interrupt sets, graphics buffers,
 * timing, and core game management functions like initialization and exit.
 * It acts as a high-level coordinator for various modules like timer, keyboard,
 * mouse, graphics, menu, and game logic.
 */

/** @defgroup manager Game Manager
 * @{
 *
 * Global variables and core functions for managing the game's lifecycle,
 * input, graphics, and overall state.
 */

#include <lcom/lcf.h>
#include "controllers/timer/timer.h"
#include "controllers/keyboardMouse/keyboard.h"
#include "controllers/keyboardMouse/mouse.h"
#include "controllers/keyboardMouse/KBC.h"
#include "controllers/video/graphics.h"
#include "menu.h"
#include "game.h"

extern uint32_t timer_irq_set;
extern uint32_t kbd_irq_set;
extern uint32_t mouse_irq_set;

extern uint8_t *frame_buffer;
extern uint8_t *second_buffer;
extern uint16_t center_x;
extern uint16_t center_y;
extern uint64_t delta_time;

/**
 * @brief Initializes the graphics subsystem.
 *
 * This function is responsible for setting the desired video mode,
 * mapping video memory, initializing drawing buffers, and preparing
 * the system for graphical output. It typically calls `set_graphic_mode`
 * from the graphics module.
 *
 * @return int 0 upon successful initialization, non-zero otherwise.
 */
int (initialize_graphics)();

/**
 * @brief Cleans up resources and exits the game.
 *
 * This function handles the graceful termination of the game. It should
 * unsubscribe all interrupts, free allocated memory (like graphics buffers),
 * restore the system to text mode, and perform any other necessary cleanup
 * before the program terminates.
 *
 * @return int 0 upon successful exit (though often this function might not return if it calls `exit()`),
 *         non-zero if an error occurs during cleanup (less common for an exit function).
 */
int (exit_game)();

/**@}*/

#endif // MANAGER_H
