#pragma once

/** @defgroup wait_player_menu wait_player_menu
 * @{
 * Functions for manipulating the waiting player menu
 */

/**
 * @brief      Function to be called on cancel button press
 */
void func_button_cancel_two();

/**
 * @brief      Initialize wait player menu
 */
void init_wait_player_menu();

/**
 * @brief      Destroy wait player menu structure and free memory used by it
 */
void destroy_wait_player_menu();

/**
 * @brief      Draw text indicating player two current status
 */
void draw_player_two_state();

/**
 * @brief      Update wait player meny
 */
void update_wait_player_menu();
