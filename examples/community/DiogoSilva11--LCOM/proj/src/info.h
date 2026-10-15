#ifndef _INFO_H_
#define _INFO_H_

#include <stdint.h>

/** @defgroup info info
 * @{
 * 
 * Constants for the game.
 */

#define GAME_MODE 0x105 /**< @brief Game video graphics mode */
#define GAME_FPS 60 /**< @brief Game frames per second */
#define CONSOLE_WIDTH 1024 /**< @brief Number of horizontal pixels */
#define CONSOLE_HEIGHT 768 /**< @brief Number of vertical pixels */

#define MENU_X_ORIGIN 450 /**< @brief Starting x position for the menu */
#define MENU_Y_ORIGIN 250 /**< @brief Starting y position for the menu */

#define KEY_1 0x82 /**< @brief Scancode break byte for key '1' */
#define KEY_0 0x8b /**< @brief Scancode break byte for key '0' */

#define KEY_ESC 0x01 /**< @brief Scancode make byte for key 'ESC' */

#define KEY_W 0x11 /**< @brief Scancode make byte for key 'W' */
#define KEY_A 0x1E /**< @brief Scancode make byte for key 'A' */
#define KEY_S 0x1F /**< @brief Scancode make byte for key 'S' */
#define KEY_D 0x20 /**< @brief Scancode make byte for key 'D' */

#define ARENA_WIDTH 22 /**< @brief the number of fences that make the arena width */
#define ARENA_HEIGHT 22 /**< @brief the number of fences that make the arena height */
#define ARENA_X_ORIGIN 204 /**< @brief Starting x position for the arena */
#define ARENA_Y_ORIGIN 60 /**< @brief Starting x position for the arena */

#define NUM_ROBOTS 10 /**< @brief Number of robots in the arena */
#define NUM_FENCES 94 /**< @brief Number of fences in the arena */

#define ENTITY_WIDTH 28 /**< @brief Horizontal pixels of an entity */
#define ENTITY_HEIGHT 25 /**< @brief Vertical pixels of an entity */
#define BULLET_WIDTH 14 /**< @brief Horizontal pixels of a bullet */
#define BULLET_HEIGHT 13 /**< @brief Vertical pixels of a bullet */

#define HERO_X_SPEED 14 /**< @brief Horizontal x speed of hero */
#define HERO_Y_SPEED 12 /**< @brief Horizontal y speed of hero */
#define ROBOT_X_SPEED 4 /**< @brief Horizontal x speed of robot */
#define ROBOT_Y_SPEED 3 /**< @brief Horizontal y speed of robot */
#define BULLET_SPEED 6 /**< @brief Speed of robot */

extern int timer_irq_set; 
extern int kbd_irq_set;
extern int mouse_irq_set;

/**@}*/

#endif /* _INFO_H */
