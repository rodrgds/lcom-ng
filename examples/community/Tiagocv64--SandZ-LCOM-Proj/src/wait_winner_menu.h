#pragma once

/** @defgroup wait_winner_menu wait_winner_menu
 * @{
 * Functions for manipulating the waiting winner menu
 */

/**
 * @brief      Function to be called on leave button press
 */
void func_button_leave();

/**
 * @brief      Initialize wait winner menu
 */
void init_wait_winner_menu();

/**
 * @brief      Destroy wait winner menu structure and free memory used by it
 */
void destroy_wait_winner_menu();

/**
 * @brief      Draw achieved score, player two score, and game state in real time
 */
void draw_player_two_scores();

/**
 * @brief      Update wait winner menu
 */
void update_wait_winner_menu();
