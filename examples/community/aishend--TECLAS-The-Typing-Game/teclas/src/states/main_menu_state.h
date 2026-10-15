/**
 * @file main_menu_state.h
 * @brief Main menu screen.
 */
#ifndef _TECLAS_MAIN_MENU_STATE_H_
#define _TECLAS_MAIN_MENU_STATE_H_

#include "../state.h"

/**
 * @brief Initialize the main menu state.
 *
 * @return 0 on success, non-zero on failure.
 */
int main_menu_state_init();

/**
 * @brief Draw the main menu.
 */
void main_menu_state_draw();

/**
 * @brief Handle a mouse click on the main menu.
 *
 * @return The GameState to transition to, or MAIN_MENU if no transition.
 */
GameState main_menu_state_handle_click();

/**
 * @brief Free resources used by the main menu state.
 */
void main_menu_state_destroy();

#endif
