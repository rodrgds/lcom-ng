#pragma once

/** @defgroup main_menu main_menu
 * @{
 * Functions for manipulating the main menu
 */

/**
 * @brief      Function to be called on play solo button press
 */
void func_button_play();

/**
 * @brief      Function to be called on play vs button press
 */
void func_button_play_two();

/**
 * @brief      Function to be called on leaderboard button press
 */
void func_button_leaderboard();

/**
 * @brief      Function to be called on exit button press
 */
void func_button_exit();

/**
 * @brief      Initialize main menu structure.
 */
void init_main_menu();

/**
 * @brief      Destroy main menu structure and free memory used by it
 */
void destroy_main_menu();

/**
 * @brief      Update main menu
 */
void update_main_menu();
