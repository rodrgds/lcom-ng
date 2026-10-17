#ifndef BUTTON_H
#define BUTTON_H

/** @defgroup ui button
 * @{
 *
 * Defines a button struct and the available operations.
 */

/**
 * @brief A general purpose button. Includes rendering and collision handling */
typedef struct Button {
  int pos_x, pos_y;
  char* text;
  int hover;
  int clicked;
  int width;
} Button;

#include <lcom/lcf.h>
#include "proj.h"
#include "driver.h"
#include "mouse_sprite.h"

/**
  * @brief Create a button struct. This initializes all the required fields.
 * 
 * @param pos_x X pos of the center of the button.
 * @param pos_y Y pos of the upper most side of the button.
 * @param text Button text.
 * @return Button Button struct generated.
 */
Button button_create(int pos_x, int pos_y, char* text);

/**
 * @brief Draw a button to the screen.
 * It's color changes according to mouse position.
 * 
 * @param button Pointer to a button struct.
 */
void button_draw(Button* button);

/**
 * @brief Process events related to a button.
 * This functions fetches the in-use mouse sprite and changes
 * the button state according to it. clicked is set to 1 if 
 * the button is being pressed.
 * 
 * @param button Pointer to a button struct.
 */
void button_handle_event(Button* button);

/**
 * @brief Call this function once per iteration. 
 * It calls button_draw and button_handle_event.
 * 
 * @param button Pointer to a button struct.
 * @param event Latest event that occured.
 */
void button_step(Button* button, Event* event);

/**@}*/

#endif
