#pragma once
#include <lcom/lcf.h>
#include "vector.h"

/**
 * @brief Struct that represents a button
 * 
 * img - xpm image of the button
 * imgSelected - xpm image of the button when it is selected
 * pos - position of the button
 * size - size of the button
 * selected - true if the button is selected, false otherwise
 *
 */
typedef struct Button {
  xpm_image_t img;
  xpm_image_t imgSelected;
  vector_t pos;
  vector_t size;
  bool selected;
} Button;

/**
 * @brief Creates a new button
 * 
 * @param img xpm image of the button
 * @param imgSelected xpm image of the button when it is selected
 * @param position position of the button
 * @param size size of the button
 * @return Button* Pointer to the button
*/
Button* newButton(xpm_image_t img, xpm_image_t imgSelected, vector_t position, vector_t size);

/**
 * @brief Frees the memory allocated for the button
 * 
 * @param button Button to be destroyed
*/
void destroyButton(Button* button);

/**
 * @brief Checks if the mouse cursor is over a button.
 *
 * This function determines if the mouse cursor, given its position, is currently over a button.
 *
 * @param button Pointer to the Button structure representing the button.
 * @param mousePos The current position of the mouse cursor as a vector.
 * @return true if the mouse cursor is over the button, false otherwise.
 */
bool isMouseOverButton(Button* button, vector_t mousePos);





