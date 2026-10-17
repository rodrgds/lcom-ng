#ifndef __MOUSE_H
#define __MOUSE_H

#include "keyboard.h"

/** @defgroup mouse mouse
 * @{
 *
 * Functions that handle the mouse part of the project
 */

/**
 * @brief Enables the Mouse stream mode and the data mode
 */
void mouse_set_stream();
/**
 * @brief Disables the Mouse stream mode
 *
 * @return 1 if success
 */
int mouse_disable_stream();
/**
 * @brief Writes information in the packets
 *
 * Reads the KBC register and checks the counter to write the information into the right packet. Also checks bit 3 to see
 * if the packets are from the same movement.
 * @return
 */
int mouse_handler();
/**
 * @brief Updates the cursor position with the input from the mouse
 *
 * With the information from the packets, the current position of the cursor will be updated.
 *
 * @param c is a struct that keeps information about the cursor of the menu, including it's current position
 * @param packets are the information read from the KBC register about the movement of the mouse
 */
void updateCursor(Cursor *c, unsigned char packets[3]);
/**
 * @brief Checks if any of the menu options were selected by the cursor
 *
 * Checks if the left button of the mouse is pressed inside the Rectangles that are the 2 options of the menu. If so is the case
 * the consequences of choosing that option will be triggered, which can be playing the game or exiting the application.
 *
 * @param menu
 */
void checkCursor(Menu *menu);


#endif
