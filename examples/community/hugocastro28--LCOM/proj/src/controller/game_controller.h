#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

/**
 * @brief Subscribes the interruptions of the devices used during the game (timer and keyboard).
 * 
 */
int (subscribe_game_devices)();

/**
 * @brief Subscribes the interruptions of the device used in the menu (mouse).
 * 
 */
int (subscribe_menu_devices)();

/**
 * @brief Unsubscribes the interruptions of the devices used during the game (timer and keyboard).
 * 
 */
int (unsubscribe_game_devices)();

/**
 * @brief Unsubscribes the interruptions of the device used in the menu (mouse).
 * 
 */
int (unsubscribe_menu_devices)();

/**
 * @brief Sets the initial position of all elements of the game, initializes the game time and all pixmaps.
 * 
 */
void (initialize_game_elements)();

#endif
