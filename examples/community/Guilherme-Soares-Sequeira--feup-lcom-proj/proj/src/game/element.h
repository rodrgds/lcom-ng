#ifndef INTERACTIVE_ELEMENT_H
#define INTERACTIVE_ELEMENT_H

#include <lcom/lcf.h>

#include <stdbool.h>

#include "../utils/position.h"
#include "canvas.h"
#include "cursor.h"
#include "draw_buttons.h"

/** @defgroup element element
 *  @{
 *
 * Constants, structures and functions used for programming the elements of our program
 */

#define COLOR_BUTTON_PRIORITY 3 /**< @brief event priority of a color button */
#define STYLE_BUTTON_PRIORITY 3 /**< @brief event priority of a style button */

/**
 * @brief Structure used to represent an interactive element
 * @param top_pos top left position of the element
 * @param h_len width of the element
 * @param v_len height of the element
 * @param color functions both as the color of the element or the style of the cursor
 * @param event_priority event priority of the element
 * @param mouse_event_handler a pointer to a function that will handle an interaction between the mouse and this element
 *
 */
typedef struct {
  position top_pos;

  uint16_t h_len;
  uint16_t v_len;

  uint8_t color;

  int8_t event_priority;

  void (*mouse_event_handler)(uint8_t color);
} interactive_element;

/**
 * @brief Returns the xpm image of the 'plus' button
 *
 * @return xpm_button_plus
 */
xpm_image_t(get_button_plus)();

/**
 * @brief Loads and creates every interactive element present on screen
 *
 */
void(load_ie_drawing)();

/**
 * @brief Indicates whether a given interactive element is being hovered by the cursor or not
 * @param elem the element being hovered
 *
 * @return true if hovered, false otherwise
 */
bool(is_hovered)(interactive_element elem);

/**
 * @brief Returns the current number of interactive elements present on screen
 *
 * @return number of interactive elements
 */
uint8_t(get_number_of_drawing_ies)();

/**
 * @brief Returns the container of the interactive elements present on screen
 *
 * @return drawing_ies
 */
interactive_element *(get_drawing_ies) ();

/**@}*/

#endif
