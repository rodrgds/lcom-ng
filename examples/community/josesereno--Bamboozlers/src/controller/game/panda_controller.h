#ifndef _PANDA_CONTROLLER_H_
#define _PANDA_CONTROLLER_H_

/**
 * @file panda_controller.h
 *
 * @brief Header file containing functions for controlling the panda character, including movement, collision detection, and animation.
 */


#include "map_controller.h"


extern Sprite* panda;
extern Sprite* little_plank;
extern Sprite* map;
extern Sprite* big_plank;
extern Sprite* little_block;
extern Sprite* big_block;
extern Sprite* item;

Sprite* collide_item;

extern Panda_state panda_state;

extern bool isRightPressed;
extern bool isLeftPressed;

/** @defgroup panda_controller panda_controller
 * @{
 */

/**
 * @brief Checks if the panda is above a given item.
 *
 * This function checks if the panda is positioned above the specified item.
 *
 * @param item The item to check.
 * @return True if the panda is above the item, false otherwise.
 */
bool (above)(Sprite* item);

/**
 * @brief Moves the panda character to the left.
 *
 * This function moves the panda character to the left.
 */
void (move_left)();

/**
 * @brief Moves the panda character to the left.
 *
 * This function moves the panda character to the right.
 */
void (move_right)();


/**
 * @brief Makes the panda character jump.
 *
 * This function makes the panda character jump by adjusting its position based on the specified time and height.
 *
 * @param time The time elapsed since the jump started.
 * @param y The initial y-coordinate of the jump.
 */
void (jump)(uint32_t time, uint32_t y);

/**
 * @brief Makes the panda character fall.
 *
 * This function makes the panda character fall by adjusting its position based on the specified time and height.
 *
 * @param time The time elapsed since the fall started.
 * @param y The initial y-coordinate of the fall.
 */
void (fall)(uint32_t time, uint32_t y);

/**
 * @brief Checks if the panda hits the floor.
 *
 * This function checks if the panda hits the floor (the bottom boundary of the map).
 *
 * @return True if the panda hits the floor, false otherwise.
 */
bool (hit_floor)();

/**
 * @brief Handles the boundary conditions for the panda character.
 *
 * This function handles the boundary conditions for the panda character, ensuring it stays within the map boundaries.
 */
void (handle_boundary_conditions)();

/**
 * @brief Checks if the panda collides with any items.
 *
 * This function checks if the panda collides with any items on the map, such as little_block, little_plank, big_plank, big_block, or map.
 * If a collision is detected, the collide_item variable is set to the collided item.
 *
 * @return True if the panda collides with an item, false otherwise.
 */
bool (collide_with_items)();

/**
 * @brief Checks if the panda is above any item.
 *
 * This function checks if the panda is positioned above any item on the map.
 *
 * @return True if the panda is above an item, false otherwise.
 */
bool (above_any_item)();

/**
 * @brief Fixes the collision between the panda and the collided item.
 *
 * This function fixes the collision between the panda and the collided item by adjusting the panda's position.
 * If the collision is on the right side of the item, the panda moves to the left; if the collision is on the left side, the panda moves to the right.
 */
void (fix_collision)();

/**
 * @brief Fixes the collision between the panda and the collided item during a jump.
 *
 * This function fixes the collision between the panda and the collided item during a jump by adjusting the panda's position.
 */
void (fix_jump_collision)();

/**
 * @brief Fixes the collision between the panda and the collided item during a fall.
 *
 * This function fixes the collision between the panda and the collided item during a fall by adjusting the panda's position.
 */
void (fix_fall_collision)();

/**
 * @brief Updates the panda character's animation frame.
 *
 * This function updates the panda character's animation frame based on its current state (JUMP or WALK) and movement direction (left or right).
 */
void (update_panda_animation)();

#endif 

