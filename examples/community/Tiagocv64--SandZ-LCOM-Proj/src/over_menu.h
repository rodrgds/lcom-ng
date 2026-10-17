#pragma once

/** @defgroup over_menu over_menu
 * @{
 * Functions for manipulating the over menu
 */

/**
 * @brief       Function to be called on retry button press
 */
void func_button_retry();

/**
 * @brief       Function to be called on main menu button press
 */
void func_button_main_menu();

/**
 * @brief      Initialize over menu structure.
 */
void init_over_menu();

/**
 * @brief      Destroy over menu structure and free memory used by it
 */
void destroy_over_menu();

/**
 * @brief      Save score of previous played game in case it is a highscore
 */
void save_highscore();

/**
 * @brief      Update over menu
 */
void update_over_menu();
