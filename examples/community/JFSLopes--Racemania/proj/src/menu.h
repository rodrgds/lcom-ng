#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>
#include "sprites.h"
#include "video_card.h"
#include "proj_main_loop.h"
#include "mouse.h"
#include "kbd.h"


/**
 * @brief Struct representing the game menu.
 */
typedef struct {
    sprite images[5]; /**< Array of sprites for the menu images */
    mouse_sprite* mouse; /**< Pointer to the mouse sprite */
    sprite menu_background; /**< Background sprite */
} Menu;

/**
 * @brief Struct representing the available races.
 */
typedef struct {
    sprite races[2]; /**< Array of sprites for the available races */
} Races;

/**
 * @brief Cleans up and frees resources used by the menu.
 */
void (clean_menu) ();
/**
 * @brief Initializes the menu with the given mouse cursor sprite and all other sprites that will be used.
 *
 * @param cursor Pointer to the mouse sprite to be used in the menu.
 * @return int 0 on success, non-zero on failure.
 */
int (initialize_menu) (mouse_sprite* cursor);
/**
 * @brief Controls the menu logic, handling input from timer, mouse, and keyboard.
 *
 * @param timer_irq Timer interrupt identifier.
 * @param mouse_irq Mouse interrupt identifier.
 * @param kbd_irq Keyboard interrupt identifier.
 * @param rtc_irq The IRQ line for the rtc.
 * @param cursor Pointer to the mouse sprite.
 * @return Game_State The current state of the game after processing the menu logic.
 */
Game_State (menu_controller) (int timer_irq, int mouse_irq, int kbd_irq, int rtc_irq, mouse_sprite* cursor);
/**
 * @brief Draws the menu on the screen.
 *
 * @return int 0 on success, non-zero on failure.
 */
int (menu_viewer_draw) ();
/**
 * @brief Draws the race selection screen.
 *
 * @return int 0 on success, non-zero on failure.
 */
int (choose_race_draw) ();
/**
 * @brief Loads the race sprites.
 */
void (load_races) ();
/**
 * @brief Checks for collision between the mouse sprite and another object sprite.
 *
 * @param mouse Pointer to the mouse sprite.
 * @param object Pointer to the object sprite.
 * @return bool True if there is a collision, false otherwise.
 */
bool (check_collision) (mouse_sprite* mouse, sprite* object);

#endif
