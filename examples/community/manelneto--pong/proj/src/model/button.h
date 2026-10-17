#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>

#include "sprite.h"

/** @defgroup button button
 * @{
 *
 * Functions related to the buttons
 */

/**
 * @brief Struct for the button
 */
typedef struct {
  int16_t x;               /*!< horizontal coordinate of the button (top-left corner) */
  int16_t y;               /*!< vertical coordinate of the button (top-left corner) */
  int16_t w;               /*!< width of the button */
  int16_t h;               /*!< height of the button */
  bool selected;           /*!< true if the button is selected, false otherwise */
  Sprite *sprite;          /*!< sprite of the button (when unselected) */
  Sprite *sprite_selected; /*!< sprite of the button (when selected) */
} Button;

/**
 * @brief Button constructor
 *
 * @param x horizontal coordinate of the button (top-left corner)
 * @param y vertical coordinate of the button (top-left corner)
 * @param w width of the button
 * @param h height of the button
 * @param xpm XPM of the button (unselected) to construct the correspondent sprite
 * @param xpm_selected XPM of the button selected to construct the correspondent sprite
 * @return Constructed button on success, NULL otherwise
 */
Button *construct_button(int16_t x, int16_t y, int16_t w, int16_t h, char *xpm[], char *xpm_selected[]);

/**
 * @brief Updates button state (selected/unselected) accordingly to cursor coordinates
 *
 * @param button pointer to button to be updated (select/unselect)
 * @param x horizontal coordinate of the cursor
 * @param y vertical coordinate of the cursor
 * @return Return 0 upon success and non-zero otherwise
 */
int check_button(Button *button, int16_t x, int16_t y);

/**
 * @brief Button destructor
 *
 * @param button pointer to the button to be destroyed
 */
void destroy_button(Button *button);

/**@}*/

#endif /* _BUTTON_H_ */
