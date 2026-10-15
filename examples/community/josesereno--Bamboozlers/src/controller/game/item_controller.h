#ifndef _ITEM_CONTROLLER_H_
#define _ITEM_CONTROLLER_H_

/**
 * @file item_controller.h
 *
 * @brief Header file containing functions for controlling and updating game items in the game.
 */

#include "map_controller.h"
#include "states/state.h"

int16_t item_x, item_y, off_x, off_y;

extern Sprite* little_plank;
extern Sprite* big_plank;
extern Sprite* bamboo;
extern Sprite* cursor;
extern Sprite* home;
extern Sprite* lava;
extern Sprite* little_block;
extern Sprite* big_block;
extern Sprite* panda;
extern Sprite* cursor;
extern Sprite* map;
extern uint8_t scancode;

/** @defgroup item_controller item_controller
 * @{
 */

Sprite* item;
/**
 * @brief Checks if an item is selected and updates the item's position.
 *
 * This function checks if any item is selected by the player and if the selected item is not already on the game map.
 * If a valid item is selected, its position is updated based on the cursor position.
 *
 * @return True if an item is selected and updated successfully, False otherwise.
 */
bool (is_selected_item)();

/**
 * @brief Checks the position of the selected item and handles collisions.
 *
 * This function checks if the selected item is within the game map bounds and if it collides with any other objects,
 * such as the panda, blocks, or planks. If a collision occurs or the item is out of bounds, the item is reset to its original position.
 */
void (check_item_pos)();

/**
 * @brief Updates the position of the selected item based on the cursor position.
 *
 * This function updates the position of the selected item by calculating the offset between the cursor position and the item's original position.
 */
void (update_item_pos)();

/**
 * @brief Updates the animation of the selected item.
 *
 * This function updates the animation frame of the selected item based on the user's input.
 * If the "R" key is pressed, the animation frame is incremented, otherwise, it is decremented.
 */
void (update_item_animation)();


/**
 * @brief Updates the inventory.
 *
 * This function updates the position of the items in the inventory.
 */
void (update_inventory)();

/**
 * @brief Selects an item based on the cursor's position.
 *
 * This function checks if any item that is in the inventory was selected by the user.
 *
 * @param item The item to check.
 * @return True if the item is selected, false otherwise.
 */
bool (select_item)(Sprite *item);

#endif 

