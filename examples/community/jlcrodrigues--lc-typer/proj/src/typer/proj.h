#ifndef PROJ_H
#define PROJ_H

/** @defgroup proj proj
 * @{
 *
 * Core of the project. Defines the main loop.
 */

#define VIDEO_MODE 0x115

/**
 * @brief Current state of the program.
 * Changing the state can by done by proj_set_state.
 * 
 */
typedef enum {MENU, GAME, GAME_OVER, OVER, SCORE} State;

#include <lcom/lcf.h>
#include "driver.h"
#include "game.h"
#include "menu.h"
#include "game_over.h"
#include "alphabet.h"
#include "mouse_sprite.h"
#include "scoreboard.h"
#include "button.h"

/**
 * @brief Initializes everything needed for the program.
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int proj_setup(void);

/**
 * @brief Runs a step of the program.
 * This should be done once per iteration. The function checks for 
 * interrupts and delegates tasks depending on the current program
 * state
 * 
 */
void proj_step(void);

/**
 * @brief Cleans up the program.
 * This function **must** be called before exiting the program.
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int proj_cleanup(void);

/**
 * @brief Step function when the state is GAME.
 * This function is needed to use on the step function pointer.
 * This is because the game step function has a different prototype
 * than required. Thus, this function acts as a proxy to the first.
 * 
 * @param event Latest event that occurred.
 */
void proj_step_game(Event event);

/**
 * @brief Step function when the state is GAME_OVER.
 * This function is needed to use on the step function pointer.
 * This is because the game_over step function has a different prototype
 * than required. Thus, this function acts as a proxy to the first.
 * 
 * @param event Latest event that occurred.
 */
void proj_step_game_over(Event event);

/**
 * @brief Changes the current state of the programing.
 * This function changes the execution state and makes the actions 
 * necessary for all the possible states.
 * 
 * @param new_state New program state.
 */
void proj_set_state(State new_state);

/**
 * @brief Get a pointer to the project's mouse sprite.
 * This can be useful to check for mouse collision e.g.
 * 
 * @return mouse_sprite* Mouse sprite struct.
 */
mouse_sprite* proj_get_mouse();

/**@}*/

#endif
