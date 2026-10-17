#pragma once

/** @defgroup pick_name_menu pick_name_menu
 * @{
 * Functions for manipulating the name picking menu
 */

/**
 * @brief      Function to be called on cancel button press
 */
void func_button_cancel();

/**
 * @brief      Function to be called on submit button press
 */
void func_button_submit();

/**
 * @brief      Receive keyboard makecode and forward to correct structure
 *
 * @param[in]  d     { makecode form keyboard }
 */
void rec_input_user(uint8_t d);

/**
 * @brief      Initialize naem pick menu
 */
void init_pick_name_menu();

/**
 * @brief      Destroy pick name menu structure and free memory used by it
 */
void destroy_pick_name_menu();

/**
 * @brief      Update name pick menu
 */
void update_pick_name_menu();
