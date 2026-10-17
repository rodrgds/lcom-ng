#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "assets/xpm/game_over/exit_button.xpm"
#include "assets/xpm/game_over/exit_button_selected.xpm"
#include "assets/xpm/game_over/game_over.xpm"
#include "assets/xpm/game_over/menu_button.xpm"
#include "assets/xpm/game_over/menu_button_selected.xpm"
#include "button.h"
#include "model/utils/sprite.h"

extern uint16_t x_res; /**< X resolution of the screen */
extern uint16_t y_res; /**< Y resolution of the screen */

/**
 * @brief Represents a single GameOver menu instance.
 *
 * The GameOver struct is opaque.
 */
typedef struct GameOver GameOver;

/**
 * @brief Creates and initializes a new GameOver instance.
 *
 * @return Pointer to the created GameOver structure.
 */
GameOver *create_game_over();

/**
 * @brief Frees the memory associated with the GameOver instance.
 *
 * @param game_over Pointer to the GameOver structure to destroy.
 * @return 0 for success, 1 for failure.
 */
int destroy_game_over(GameOver *game_over);

/**
 * @brief Gets the currently selected button in the game over menu.
 *
 * @param game_over Pointer to the GameOver structure.
 * @return The type of the currently selected button.
 */
ButtonType game_over_get_button(GameOver *game_over);

/**
 * @brief Cycles to the next selectable button in the game over menu.
 *
 * @param game_over Pointer to the GameOver structure.
 * @return 0 for success, 1 for failure.
 */
int game_over_change_button(GameOver *game_over);

/**
 * @brief Handles a mouse click and determines if a button was clicked.
 *
 * @param game_over Pointer to the GameOver structure.
 * @param x X-coordinate of the mouse click.
 * @param y Y-coordinate of the mouse click.
 * @return The type of the button that was clicked, or BUTTON_NONE if no button was clicked.
 */
ButtonType game_over_click_handler(GameOver *game_over, uint16_t x, uint16_t y);

/**
 * @brief Draws the game over menu on the given frame buffer.
 *
 * @param game_over Pointer to the GameOver structure.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 for success, 1 for failure.
 */
int draw_game_over(GameOver *game_over, uint8_t *frame_buffer);

#endif /* GAME_OVER_H */
