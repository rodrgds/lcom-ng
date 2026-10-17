#pragma once

/** @defgroup pause_menu pause_menu
 * @{
 * Functions for manipulating the pause menu
 */

/**
 * @brief      Function to be called on resume button press
 */
void func_button_resume();

/**
 * @brief      Function to be called on quit button press
 */
void func_button_quit();

/**
 * @brief      Initialize over menu structure.
 */
void init_pause_menu();

/**
 * @brief      Destroy pause menu structure and free memory used by it
 */
void destroy_pause_menu();

/**
 * @brief      Function to generate current game blured background with O(n) complexity
 */
void load_background_blur();

/**
 * @brief      Draw previous generated blured background
 */
void draw_background_blur();

/**
 * @brief      Update mouse menu
 */
void update_pause_menu();
