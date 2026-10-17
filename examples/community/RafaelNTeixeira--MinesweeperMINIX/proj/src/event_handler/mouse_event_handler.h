#pragma once

#include <stdio.h>
#include "devices/mouse/mouse.h"
#include "game/game_state.h"

/**
 * @brief This function handles mouse events based on the current program state.
 * @param pp A 'struct packet' containing the information about the mouse event.
*/
void handle_mouse_event(struct packet pp);

/**
 * @brief This function handles the mouse event that occurs in the beginner game mode.
 * @param pp A 'struct packet' containing the information about the mouse event.
*/
void handle_mouse_beginner_event(struct packet pp);

/**
 * @brief This function handles the mouse event that occurs in the medium game mode.
 * @param pp A 'struct packet' containing the information about the mouse event.
*/
void handle_mouse_medium_event(struct packet pp);

/**
 * @brief This function handles the mouse event that occurs in the hard game mode.
 * @param pp A 'struct packet' containing the information about the mouse event.
*/
void handle_mouse_hard_event(struct packet pp);

/**
 * @brief This function handles mouse events specifically in the "MAIN_MENU" state.
 * @param pp A 'struct packet' containing the information about the mouse event.
*/
void handle_mouse_menu_event(struct packet pp);

/**
 * @brief This function handles the mouse event that occurs in the game win state.
 * @param pp A 'struct packet' containing the information about the mouse event.
*/
void handle_mouse_gamewin_event(struct packet pp);

/**
 * @brief This function handles the mouse event that occurs in the game over state.
 * @param pp A 'struct packet' containing the information about the mouse event.
*/
void handle_mouse_gameover_event(struct packet pp);

/**
 * @brief This function handles the mouse event that occurs in the instructions state. 
 * @param pp A 'struct packet' containing the information about the mouse event.
*/
void handle_mouse_instruction_event(struct packet pp);
