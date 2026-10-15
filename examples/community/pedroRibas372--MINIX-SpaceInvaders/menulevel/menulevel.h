#ifndef MENULEVEL_H
#define MENULEVEL_H

#include "../sprites/sprites.h"

/**
 * @brief Represents the different level options available in the game.
 *
 */
typedef enum {
    /**
     * @brief First level of the game.
     */
    LEVEL1,

    /**
     * @brief Second level of the game.
     */
    LEVEL2,

    /**
     * @brief Third level of the game.
     */
    LEVEL3,

    /**
     * @brief Option to exit the level selection menu.
     */
    LEVEL_EXIT,  

    /**
     * @brief Return 0 upon success and non-zero otherwise.
     */
    LEVEL_COUNT  
} LevelSelection;


/**
 * @brief Runs the level selection menu loop.
 *
 * This function displays the level selection screen, handles user input via keyboard interrupts,
 * and updates the currently selected level.
 * 
 *  The user can navigate using the arrow keys and select a level
 * with the Enter key or cancel with the Escape key. 
 * 
 * Upon selection, the corresponding level is saved in
 * `selected_level`, and the game returns to the main menu (`MENU_MODE`).
 *
 * @return int Returns 0 on success, or 1 if an error occurs (e.g., during sprite creation or
 * keyboard interrupt subscription/unsubscription).
 */
int level_menu_loop();

#endif
