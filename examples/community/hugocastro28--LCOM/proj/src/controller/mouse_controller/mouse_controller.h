#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

/**
 * @brief Checks if the mouse is clicked over any menu option
 * and if it is, display the corresponding result.
 * 
 */
void (check_button)(bool clicked);

/**
 * @brief Updates the mouse cursor position and redraws it.
 * 
 */
void (update_cursor)(struct packet pp);

/**
 * @brief Checks if complete mouse information (3 packets) has already been sent, 
 * and if so, updates the cursor position.
 * 
 */
void (mouse_interrupt_handler)();

#endif
