#include <stdio.h>
#include "mouse.h"
#include "game_state.h"

/**
 * @brief Handles generic mouse events.
 *
 * This function processes mouse input events for the generic state of the game or
 * application, updating the state based on the provided packet data.
 *
 * @param pp A structure containing the packet data from the mouse.
 */
void handle_mouse_event(struct packet pp);

/**
 * @brief Handles mouse events during the game.
 *
 * This function processes mouse input events specifically when the game is in
 * the playing state, updating the game's state based on the provided packet data.
 *
 * @param pp A structure containing the packet data from the mouse.
 */
void handle_mouse_game_event(struct packet pp);

/**
 * @brief Handles mouse events during the main menu.
 *
 * This function processes mouse input events specifically when the game is in
 * the main menu, updating the menu's state based on the provided packet data.
 *
 * @param pp A structure containing the packet data from the mouse.
 */
void handle_mouse_menu_event(struct packet pp);

/**
 * @brief Handles mouse events during the instructions screen.
 *
 * This function processes mouse input events specifically when the game is displaying
 * instructions, updating the state based on the provided packet data.
 *
 * @param pp A structure containing the packet data from the mouse.
 */
void handle_mouse_instructions_event(struct packet pp);
