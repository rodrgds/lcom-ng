#ifndef LEVEL_H
#define LEVEL_H
#include "../macros/includes.h"
#include "../load/view.h"
#include "../load/spawner.h"
#include "../devices/mouse.h"
#include "../Entities/player.h"
#include "../Entities/enemies_lvl.h"
#include "../Entities/workers.h"
#include "../Entities/wolf.h"
#include "../Entities/bullet.h"
#include "../Entities/teleport.h"
#include "../Entities/inventory.h"

/**
 * @brief Controls the inputs of the keyboard to control the level state
 * @param scancode Keyboard input which will control the level state and it's features
 * @return Returns the action that has been made by the user using the keyboard
 */
action_t keyboard_ih_level(uint32_t scancode);
/**
 * @brief Controls the inputs of the mouse to control the level state
 * @return Returns the action that has been made by the user using the mouse
 */
action_t mouse_ih_level();

/**
 * @brief Drawing function that draws all the features of the level state in a non moving state.
 */
void draw_afk_level();
/**
 * @brief Drawing function that draws all the features of the level state.
 */
void draw_level();
/**
 * @brief Draws all the level letters and symbols
 */
void draw_level_words();
/**
 * @brief Draws all the level numbers
 */
void draw_level_numbers();
/**
 * @brief Draws all the items of the level
 */
void draw_items();
#endif
