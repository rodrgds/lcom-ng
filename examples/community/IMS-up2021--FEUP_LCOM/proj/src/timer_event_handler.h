#include <stdio.h>
#include "timer.h"
#include <lcom/timer.h>
#include "interrupt_manager.h"
#include "game_state.h"


/**
 * @brief Handles timer events.
 *
 * This function is responsible for processing timer events in the game or application.
 * It typically includes tasks such as updating game state, advancing animations, 
 * or handling time-based events.
 */
void handle_timer_event();

/**
 * @brief Handles the menu state.
 *
 * This function is responsible for processing events and logic specific to the menu state.
 * It may include tasks such as displaying menu options, handling user input to navigate 
 * the menu, or initiating transitions to other states.
 */
void handle_menu();

/**
 * @brief Handles the game state.
 *
 * This function is responsible for processing events and logic specific to the game state.
 * It includes tasks such as updating game objects, handling user input during gameplay, 
 * and managing game mechanics.
 */
void handle_game();

/**
 * @brief Handles the instructions state.
 *
 * This function is responsible for processing events and logic specific to the instructions state.
 * It may include tasks such as displaying instructions to the user, handling user input 
 * to navigate the instructions, or transitioning to other states.
 */
void handle_instructions();

