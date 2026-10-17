#ifndef MOUSE_SPRITE_H
#define MOUSE_SPRITE_H

#include <lcom/lcf.h>

/** @defgroup ui mouse_sprite
 * @{
 * 
 *  Mouse cursor related code.
 */

#include "driver.h"
#include "video.h"
#include "mouse_xpm.xpm"
#include "sprite.h"

#define MOUSE_START 300

/**
 * @brief Visible cursor on the screen.
 * The structs holds everything needed to display an move a mouse
 * cursor. Use "::"<mouse_sprite_step> to update it.
 */
typedef struct mouse_sprite {
  int16_t x_pos;
  int16_t y_pos;
  int clicked;
} mouse_sprite;

/**
 * @brief Create a mouse sprite.
 * 
 * @param mouse mouse_sprite struct passed by reference. This will be initialized.
 */
void mouse_sprite_create(mouse_sprite* mouse);

/**
 * @brief Handle an event and update the mouse accordingly.
 * Should only be used by mouse_sprite_step.
 * 
 * @param mouse Mouse object.
 * @param event Event to be handled.
 */
void mouse_sprite_handle_event(mouse_sprite* mouse, Event event);

/**
 * @brief Draw the cursor on the screen.
 * 
 * @param event Event to be handled.
 */
void mouse_sprite_draw(mouse_sprite* mouse);

/**
 * @brief Handles events and updates the cursor on the screen.
 * Only mouse events are deal with. Nevertheless, the drawer is allays called.
 * 
 * @param mouse Mouse object.
 * @param event Event to be handled.
 */
void mouse_sprite_step(mouse_sprite* mouse, Event event);

/**@}*/

#endif
