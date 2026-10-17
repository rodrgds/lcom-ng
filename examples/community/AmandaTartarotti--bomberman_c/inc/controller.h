#ifndef _LCOM_CONTROLLER_H_
#define _LCOM_CONTROLLER_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "graphic.h"
#include "timer.h"
#include "video.h"
#include "keyboard.h"
#include "rtc.h"
#include "mouse.h"
#include "sprite.h"
#include "setup_game.h"
#include <stdlib.h>


/**
 * @brief Defines conditions for breaking the driver while loop.
**/
bool breakCondition();
/**
 * @brief Sets up all devices for the game.
 * Subscribes to interrupts for the timer, keyboard, mouse, and RTC.
 * Maps VRAM and sets its mode according to user input (0x115 or 0x14C).
 * 
 * @return Returns 0 upon success and non-zero otherwise.
**/
int setup();
/**
 * @brief Destroys all sprites and unsubscribes from all devices.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
**/
int setdown();
/**
 * @brief Increases the hero's life, if the current value is less than or equal to 3.
**/
void increase_life();
/**
 * @brief Decreases the hero's life.
**/
void decrease_life();
/**
 * @brief Handles game actions upon mouse interrupt. Based on the direction of the mouse
 * and the buttons clicked, the function acts accordingly, considering different elements on the map.
 * 
 * For example, the hero can move to the right until there are no walls in the way.
 * 
 * @param direction Indicates the mouse's movement direction or if the right button was clicked.
 * @return Returns 0 upon success and non-zero otherwise.
**/
int mouse_response(int direction);
/**
 * @brief Handles game actions upon keyboard interrupt. Based on the key pressed,
 * the function acts accordingly, considering different elements on the map.
 * 
 * For example, the hero can move to the right until there are no walls in the way.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
**/
int gamepad_response();
/**
 * @brief Checks the state of the bomb_state state machine. Defines the events of the bomb 
 * according to its current state. Also considers the position where the bomb was dropped
 * and uses the timed variable (defined in the timer module) to change states according to time passed.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
**/
int check_bomb();
/**
 * @brief Checks the state of the unlimited_bomb_state state machine. Defines the events of the bombs 
 * according to their current state. Also considers the position where the bombs were dropped
 * and uses the timed variable (defined in the timer module) to change states according to time passed.
 * 
 * A for loop is used to go through all the states of all the unlimited bombs dropped when the user acquired the upgrade.
 * @return Returns 0 upon success and non-zero otherwise.
**/
int check_unlimited_bombs();

/**
 * @brief Defines the movement of green monsters (slower monsters) based on a random variable.
 * 
 * Depending on a randomly defined integer variable, the monster will move in a direction if it is free. 
 * Otherwise, it will stay in its current place.
 * 
 * A for loop is used to define the movement of each green monster.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
**/
int move_monsters_green();

/**
 * @brief Defines the movement of red monsters (faster monsters) based on a random variable.
 * 
 * Depending on a randomly defined integer variable, the monster will move in a direction if it is free. 
 * Otherwise, it will stay in its current place.
 * 
 * A for loop is used to define the movement of each red monster.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
**/
int move_monsters_red();
/**
 * @brief Defines which element will be drawn upon an explosion of a breakable object.
 * There is a 10% chance of drawing an additional heart or an unlimited bombs upgrade.
 * If no upgrade is drawn, it will draw the explosion itself, which will then be converted to a free space on the map.
**/
void get_upgrade(GamePos position);
/**
 * @brief Defines the hero's actions based on their position relative to other game elements.
 * 
 * For example, if the hero is next to an additional life, the life count will be incremented
 * and a red heart will be drawn on the user interface.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
**/
int hero_actions();
/**
 * @brief Updates the position of the mouse arrow, considering the mouse displacement and screen resolution.
 * 
 * @param delta_x Used for calculating the current horizontal position relative to the last position.
 * @param delta_y Used for calculating the current vertical position relative to the last position.
**/
void arrow_update(int16_t delta_x, int16_t delta_y);
/**
 * @brief Checks the game state to determine if the user has won or lost.
 * 
 * This function is constantly called by the timer_update() function to ensure instant event detection.
**/
void check_game();

#endif
