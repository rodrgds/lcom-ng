/**
 * @file bg.h
 * @brief Header file for background management in the game.
 */

#ifndef BG_H
#define BG_H

#include <lcom/lcf.h>

/**
 * @brief Draws the main menu background.
 * @details This function draws the main menu background on the screen.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_mmbg();


/**
 * @brief Deletes the main menu background.
 * @details This function deletes the main menu background from the screen.
 * @return 0 upon success, non-zero otherwise.
 */
int delete_mmbg();


#endif
