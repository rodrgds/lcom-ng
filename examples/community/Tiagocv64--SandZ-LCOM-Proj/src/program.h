#pragma once

/** @defgroup program program
 * @{
 * Functions for manipulating the program
 */

/**
 * @brief      Initialize program structure
 */
void init_program();

/**
 * @brief      Destroy program structure and free memory used by it
 */
void destroy_program();

/**
 * @brief      Handle timer interrupt relating to project
 */
void handle_timer_interrupt_proj();

/**
 * @brief      Handle keyboard interrupt relating to project
 */
void handle_keyboard_interrupt_proj();

/**
 * @brief      Handle mouse interrupt relating to project
 *
 * @param      mouse_pak  { mouse packet with mouse information }
 */
void handle_mouse_interrupt_proj(struct packet *mouse_pak);
