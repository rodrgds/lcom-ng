#ifndef MENU_H
#define MENU_H

typedef enum {
    MENU_START,
    MENU_LEVEL,
    MENU_EXIT
} MenuSelection;


/**
 * @brief Updates the state of the menu buttons based on user navigation.
 *
 * This function changes the graphical representation of the start, level, and exit
 * buttons depending on which one is currently selected.
 */
void updateButtonStates();

/**
 * @brief Handles keyboard interrupts and user input while in the main menu.
 *
 * This function subscribes to keyboard interrupts and waits for user input
 * (arrow keys, Enter, or Escape) to navigate the main menu. It updates the
 * `current_button` global variable, changes the game state depending on selection, and may call
 * the level selection menu. 
 * 
 * 
 * Exits the loop when the game leaves `MENU_MODE`.
 *
 * @return Return 0 upon success and non-zero otherwise `
 */
int controllMenuInterrupts();

/**
 * @brief Initializes and displays the main menu.
 *
 * This function creates the menu elements, 
 * and enters the input handling loop via `controllMenuInterrupts()`.
 * Once the menu exits, all allocated sprites are destroyed to free memory.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int menu_loop();


#endif
