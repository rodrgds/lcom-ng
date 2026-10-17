#ifndef VICTORY_H
#define VICTORY_H

#include <lcom/lcf.h>

#include "assets/xpm/victory/exit_button.xpm"
#include "assets/xpm/victory/exit_button_selected.xpm"
#include "assets/xpm/victory/next_button.xpm"
#include "assets/xpm/victory/next_button_selected.xpm"
#include "assets/xpm/victory/victory.xpm"
#include "model/menu/button.h"
#include "model/utils/sprite.h"

extern uint16_t x_res; /**< X resolution of the screen */
extern uint16_t y_res; /**< Y resolution of the screen */

/**
 * @brief Represents a single Victory menu instance.
 *
 * The Victory struct is opaque.
 */
typedef struct Victory Victory;

/**
 * @brief Creates and initializes a new Victory instance.
 *
 * @return Pointer to the created Victory structure.
 */
Victory *create_victory();

/**
 * @brief Frees the memory associated with the Victory instance.
 *
 * @param victory Pointer to the Victory structure to destroy.
 * @return 0 for success, 1 for failure.
 */
int destroy_victory(Victory *victory);

/**
 * @brief Gets the currently selected button in the victory menu.
 *
 * @param victory Pointer to the Victory structure.
 * @return The type of the currently selected button.
 */
ButtonType victory_get_button(Victory *victory);

/**
 * @brief Cycles to the next selectable button in the victory menu.
 *
 * @param victory Pointer to the Victory structure.
 * @return 0 for success, 1 for failure.
 */
int victory_change_button(Victory *victory);

/**
 * @brief Handles a mouse click and determines if a button was clicked.
 *
 * @param victory Pointer to the Victory structure.
 * @param x X-coordinate of the mouse click.
 * @param y Y-coordinate of the mouse click.
 * @return The type of the button that was clicked, or BUTTON_NONE if no button was clicked.
 */
ButtonType victory_click_handler(Victory *victory, uint16_t x, uint16_t y);

/**
 * @brief Draws the victory menu on the given frame buffer.
 *
 * @param victory Pointer to the Victory structure.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 for success, 1 for failure.
 */
int draw_victory(Victory *victory, uint8_t *frame_buffer);

#endif
