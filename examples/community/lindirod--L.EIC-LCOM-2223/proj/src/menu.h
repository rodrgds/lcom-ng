/**
 * @file menu
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 * 
 *
 * @brief This file contains the declarations of the functions used for the menu screen of the game.
 * 
 */
#ifndef _MENU_H_
#define _MENU_H_


/**
 * 
 * @brief Initializes the necessary XPM image files for the menu screen.
 * 
 */
void (menu_files_init)();

/**
 * 
 * @brief Draws the menu screen.
 * 
 */
void (draw_menu)();

/**
 * @brief This function navigates the menu screen based on the user input.
 * 
 * @param scancode The scancode of the user input
 * 
 * @return True if the navigation was successful; False otherwise
 * 
 */
bool (navigate_menu)(uint8_t *scancode);

/**
 * @brief Handles mouse events on the menu screen.
 * 
 * @param scancode The scancode of the user input
 * @param cur_mouse_ev The current mouse event
 * @return true if the mouse event was handled, false otherwise
 */
bool menu_mouse_handler(uint8_t *scancode, struct mouse_ev *cur_mouse_ev);

/**
 * @brief Updates the mouse coordinates on the menu screen
 * 
 * @param draw Whether to draw the mouse or not
 * 
 */
void (update_mouse_coordinates)(bool draw);

/**
 * 
 * @brief This function clears the XPM images for the menu screen
 * 
 */
void (clear_menu)();

/**
 * @brief Initializes the necessary XPM image files for the menu screen
 * 
 */
void (instruction_file_init)();

/**
 * @brief Draws the instructions
 * 
 */
void (draw_instructions)();

/**
 * @brief This function clears the XPM images for the instruction
 * 
 */
void (clear_instructions)();


#endif // _MENU_H_
