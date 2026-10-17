#pragma once

#include <stdio.h>
#include "devices/timer/timer.h"
#include <lcom/timer.h>
#include "devices/int_manager.h"
#include "game/game_state.h"
#include "game/display.h"

/**
 * @brief This function handles timer events.
*/
void handle_timer_event();

/**
 * @brief This function handles timer events in the "MAIN_MENU" state.
*/
void handle_timer_menu_event();

/**
 * @brief This function updates the game state and performs necessary actions based on the timer event during beginner gameplay.
*/
void handle_timer_beginner_event();

/**
 * @brief This function updates the game state and performs necessary actions based on the timer event during medium gameplay.
*/
void handle_timer_medium_event();

/**
 * @brief This function updates the game state and performs necessary actions based on the timer event during hard gameplay.
*/
void handle_timer_hard_event();


/**
 * @brief This function handles timer events in the "INSTRUCTIONS" state.
*/
void handle_timer_instructions_event();

/**
 * @brief This function updates the game state and performs necessary actions based on the timer event when the game is won.
*/
void handle_timer_gamewin_event();

/**
 * @brief This function updates the game state and performs necessary actions based on the timer event when the game is over.
*/
void handle_timer_gameover_event();
