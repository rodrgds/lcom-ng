#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include "../macros/includes.h"
#include "../load/view.h"
#include "../devices/graphics.h"
#include "../devices/keyboard.h"
#include "../devices/mouse.h"
#include "../devices/timer_proj.h"
#include "../game_state/menu.h"
#include "../game_state/level.h"
#include "../game_state/pause.h"
#include "../game_state/game_over.h"
#include "../load/spawner.h"
#include "../devices/rtc.h"

/**
 * @brief Changes the current value of the state of the struct game_state_t
 * @param state The state to which the state of the game is going to change into
 */
void set_state(game_state_t state);

/**
 * @brief Calls the keyboard interrupt handlers of the current state
 * @param scancode Keyboard input value
 */
int keyboard_ih(uint32_t scancode);
/**
 * @brief Calls the mouse interrupt handlers of the current state
 */
int mouse_proj_ih();

/**
 * @brief Calls set_state function whenever a certain action is made in a current state
 * @return Returns true when the user quits the game and false when he doesn't
 */
bool control_state();
/**
 * @brief Calls the drawing functions of the current state
 * @return Returns 0 if successfull and 1 if current state isn't available
 */
int draw_state();

/**
 * @brief Initializes all the devices, Calls functions that load and store xpms
 * @return Returns 0 if successfull and 1 otherwise
 */
int initialize();
/**
 * @brief Game loop where the program registers all the interrupts and acts accordingly depending on which one was utilized
 * @return Returns 0 if successfull and 1 otherwise
 */
int game_loop();

/**
 * @brief Terminates all devices and frees buffer memory space
 * @return Returns 0 if successfull and 1 otherwise
 */
int terminate();
#endif


