/**
 * @file sprites.h
 * @brief Header file for sprite loading and handling functions.
 */

#ifndef SPRITES_H
#define SPRITES_H

#include <lcom/lcf.h>

/**
 * @brief Load all game sprites based on the current time (hours).
 * @details This function loads the appropriate sprites for the game based on whether it is day or 
 * night. It uses the global variable `hours` to determine the time of day.
 */
void (load_xpm)();

#endif
