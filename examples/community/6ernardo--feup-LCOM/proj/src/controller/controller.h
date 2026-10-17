#ifndef _LCOM_CONTROLLER_H_
#define _LCOM_CONTROLLER_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "model/model.h"
#include "keyboard/keyboard.h"
#include "mouse/mouse.h"
#include "keyboard/i8042.h"
#include "view/view.h"

/**
 * @brief Handles user input and updates the drawing controls accordingly.
 * @param scancode Scancode of the key pressed.
 * The draw_controls() function is responsible for handling user input and updating the drawing controls based on the provided
 * scancode. It uses a switch statement to determine the action to be taken based on the scancode.
 */

void draw_controls(uint8_t scancode);

/**
 * @brief Checks if the mouse cursor is over a sprite at the specified position.
 *
 * The mouse_over_sprite() function determines whether the current position of the mouse cursor is within the boundaries of a sprite. It compares the mouse coordinates with the given position (x, y) and the dimensions of the sprite.
 *
 * @param sprite Pointer to the sprite to check.
 * @param x The x-coordinate of the top-left corner of the sprite's bounding box.
 * @param y The y-coordinate of the top-left corner of the sprite's bounding box.
 *
 * @return 1 if the mouse cursor is over the sprite, 0 otherwise.
 */

int mouse_over_sprite(Sprite *sprite, int x, int y);

/**
 * @brief Updates the drawing color based on the user's selection.
 *
 * The pressed_color() function is responsible for updating the drawing color based on the color selected by the user. It checks if the left mouse button is clicked (mouse_info.left_click). If the left mouse button is clicked, it calls the mouse_over_color() function to determine which color is selected.
 */
void pressed_color();

/**
 * @brief Determines the color index based on the mouse position.
 *
 * The mouse_over_color() function checks the current mouse position and determines the corresponding color index based on the x and y coordinates. It returns the color index as an integer value.
 *
 * @return The color index based on the mouse position. Returns 0 if the mouse position is outside the color selection area.
 */


int mouse_over_color();

/**
 * @brief Checks if the mouse is over a clickable element.
 *
 * The mouse_over_clickable() function checks the current game state and determines if the mouse cursor is over a clickable element based on the game state.
 *
 * @return 0 if the mouse is over a clickable element, 1 otherwise.
 */
int mouse_over_clickable();

/**
 * @brief Checks if the draw button is pressed.
 *
 * The draw_button_pressed() function checks whether the mouse cursor is over the draw button sprite at the specified position and determines if the left mouse button is clicked.
 *
 * @return 0 if the draw button is pressed, 1 otherwise.
 */

int draw_button_pressed();

/**
 * @brief Checks if the quit button is pressed.
 * @return 0 if the quit button is pressed, 1 if the quit button is released.
 *
 * The quit_button_pressed() function checks if the quit button sprite is pressed by the mouse cursor. It uses the mouse_over_sprite() function to determine if the mouse cursor is over the button_quit sprite at the specified coordinates (250, 400).
 *
 * If the mouse cursor is over the button and the left mouse button is pressed (mouse_info.left_click is true), the function sets quit_pressed to true and returns 0 to indicate that the quit button has been pressed.
 *
 * If quit_pressed is true (indicating that the quit button was previously pressed) and the left mouse button is released (mouse_info.left_click is false), the function sets quit_pressed to false and returns 1 to indicate that the quit button has been released.
 */


int quit_button_pressed();

/**
 * @brief Checks if the help button is pressed and handles its state.
 * @return 1 if the button is not pressed or if it was pressed but is no longer being clicked, 0 if the button was just pressed.
 */

int help_button_pressed();

/**
 * @brief Checks if the arrow button is pressed and handles its state.
 * @return 1 if the button is not pressed or if it was pressed but is no longer being clicked, 0 if the button was just pressed.
 */
int arrow_button_pressed();

/**
 * @brief Fills a region with the current drawing color using the paint bucket tool.
 */

void paint_bucket();

#endif
